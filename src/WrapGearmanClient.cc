

#include "WrapGearmanClient.h"
#include "BackgroundTask.h"
#include "Baton.h"

#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdio>
using namespace std;

Persistent<Function> WrapGearmanClient::constructor;

bool foo(const Baton* item) {
	return item->isDone();
}

class MyExecute : public NanAsyncProgressWorker {
public:
	WrapGearmanClient* gClient;

	explicit MyExecute(WrapGearmanClient *gClient_)
		: NanAsyncProgressWorker(new NanCallback()), gClient(gClient_)  { }

	void Execute (const ExecutionProgress& progress) {
		while(gClient->running) {

			gClient->debug && printf("%s\n", "-- sleeping...");
			usleep(500);
			gClient->debug && printf("%s\n", "-- unslept");

			if (uv_mutex_trylock(&gClient->client_mutex) != 0) {
				continue;
			}

			gClient->debug && printf("%s %d\n", "-- size:", (int) gClient->tasks.size());

			for(list<Baton*>::iterator it = gClient->tasks.begin(); it != gClient->tasks.end(); it ++) {
				Baton* el = *it;
				if (el->isDone()) { continue; }
				el->Execute(gClient);
				el->setToDone();

				if (!el->isOk()) {
					break;
				}
			}
			gClient->unlockClient();

			gClient->debug && printf("%s\n", "-- seding...");
			progress.Send(NULL, 0);
			gClient->debug && printf("%s\n", "-- sent");
		}
	}

	void HandleOKCallback () {
		NanScope();
		Local<Value> argv[0] = { };
		gClient->endCallback->Call(0, argv);
	}

	void HandleProgressCallback(const char *data, size_t size) {
		gClient->debug && printf("Calling %d task callbacks\n", (int) gClient->tasks.size());

		Baton* el;
		gClient->lockClient();
		for (list<Baton*>::iterator it = gClient->tasks.begin(); it != gClient->tasks.end(); it++) {
			el = *it;
			if (!el->isDone()) { continue; }
			el->invokeCallback();
			gClient->debug && printf("%s\n", "callback invoked");
		}
		gClient->tasks.remove_if(foo);
		gClient->unlockClient();
		/*
		for (list<Baton*>:	:iterator it = toRemove.begin(); it != toRemove.end(); it++) {
			gClient->tasks.remove(*it);
		}
		*/
	}
};

WrapGearmanClient::WrapGearmanClient() {
	client = gearman_client_create(NULL);
	gearman_client_set_timeout(client, 1000);

	uv_mutex_init(&client_mutex);
	debug = false;
	running = true;

	NanAsyncQueueWorker(new MyExecute(this));
}

void WrapGearmanClient::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew<String>("WrapGearmanClient"));

	NODE_SET_PROTOTYPE_METHOD(t, "setDebug", setDebug);
	NODE_SET_PROTOTYPE_METHOD(t, "addServer", addServer);
	NODE_SET_PROTOTYPE_METHOD(t, "doBackground", doBackground);
	NODE_SET_PROTOTYPE_METHOD(t, "stop", stop);
	NODE_SET_PROTOTYPE_METHOD(t, "getStatus", getStatus);

	NanAssignPersistent(constructor, t->GetFunction());
	exports->Set(NanNew("WrapGearmanClient"), t->GetFunction());
}

NAN_METHOD(WrapGearmanClient::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		return NanThrowTypeError("Use the new operator to create new WrapGearmanClient objects");
	}

	WrapGearmanClient* gClient = new WrapGearmanClient();
	gClient->Wrap(args.This());

	NanReturnValue(args.This());
}

NAN_METHOD(WrapGearmanClient::doBackground) {
	NanScope();
	NanCallback *callback = new NanCallback(args[1].As<Function>());
	BackgroundTask* task = ObjectWrap::Unwrap<BackgroundTask>(args[0]->ToObject());
	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());

	Baton* baton = new JobBackgroundBaton(task, callback);
	gClient->tasks.push_back(baton);

	NanReturnValue(args.This());
}

NAN_METHOD(WrapGearmanClient::stop) {
	NanScope();
	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());
	gClient->endCallback = new NanCallback(args[0].As<Function>());
	gClient->running = false;

	NanReturnValue(args.This());
}

NAN_METHOD(WrapGearmanClient::addServer) {
	NanScope();

	if (args.Length() <= 0 || !args[0]->IsString()) {
		return NanThrowTypeError("Argument 0 must be a string");
	}
	String::Utf8Value str(args[0]->ToString());
	if (args.Length() <= 1 || !args[1]->IsUint32()) {
		return NanThrowTypeError("Argument 1 must be a integer");
	}
	int port = args[1]->Int32Value();

	char* host = new char[str.length()];
	strcpy(host, *str);

	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());

	gClient->debug && printf("%s %s %d\n", "addServer", *str, port);
	gearman_return_t ret = gearman_client_add_server(gClient->client, host, (in_port_t) port);

	NanReturnValue(NanNew<Boolean>(gearman_success(ret)));
}

void log(const char *line, gearman_verbose_t verbose, void *context) {
	printf("%s %s\n", gearman_verbose_name(verbose), line);
}

NAN_METHOD(WrapGearmanClient::setDebug) {
	NanScope();

	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());
	if (args.Length() == 1) {
		gClient->debug = args[0]->BooleanValue();
	}

	gearman_client_set_log_fn(gClient->client, log, NULL, GEARMAN_VERBOSE_MAX);

	NanReturnValue(NanNew<Boolean>(true));
}

NAN_METHOD(WrapGearmanClient::getStatus) {
	NanScope();

	if (args.Length() <= 0 || !args[0]->IsString()) {
		return NanThrowTypeError("Argument 0 must be a string");
	}
	String::Utf8Value str(args[0]->ToString());

	if (args.Length() <= 1 || !args[1]->IsFunction()) {
		return NanThrowTypeError("Argument 1 must be a function");
	}
	NanCallback *callback = new NanCallback(args[1].As<Function>());

	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());
	Baton* baton = new JobStatusBaton(*str, callback);
	gClient->tasks.push_back(baton);

	NanReturnValue(args.This());
}


void WrapGearmanClient::lockClient() {
	uv_mutex_lock(&client_mutex);
}

void WrapGearmanClient::unlockClient() {
	uv_mutex_unlock(&client_mutex);
}
