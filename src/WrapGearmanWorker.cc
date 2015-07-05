
#include "WrapGearmanWorker.h"

#include <unistd.h>

Persistent<Function> WrapGearmanWorker::constructor;


class GrabJobWorker : public NanAsyncProgressWorker {
public:
	WrapGearmanWorker* gWorker;

	explicit GrabJobWorker(WrapGearmanWorker *gWorker_)
		: NanAsyncProgressWorker(new NanCallback()), gWorker(gWorker_)  { }

	void Execute (const ExecutionProgress& progress) {
		while(gWorker->running) {

			gWorker->debug && printf("%s\n", "-- sleeping...");
			usleep(500);
			gWorker->debug && printf("%s\n", "-- unslept");
			gWorker->debug && printf("%s %d\n", "-- size:", (int) gWorker->jobs.size());

			GearmanJob* job = new GearmanJob();
			job->job = gearman_worker_grab_job(gWorker->worker, NULL, job->ret);
			gWorker->debug && printf("%s\n", "-- grabbed");

			gWorker->jobs.push_back(job);

			gWorker->debug && printf("%s\n", "-- seding...");
			progress.Send(NULL, 0);
			gWorker->debug && printf("%s\n", "-- sent");
		}
	}

	void HandleOKCallback () {
		NanScope();
		Local<Value> argv[0] = { };
		gWorker->endCallback->Call(0, argv);
	}

	void HandleProgressCallback(const char *data, size_t size) {
		/*
		gWorker->debug && printf("Calling %d task callbacks\n", (int) gWorker->tasks.size());
		int n = gWorker->tasks.size();
		list<Baton*>::iterator it = gWorker->tasks.begin();

		for (int i = 0; i < n ; i ++ ) {
			Baton* el = *it;
			if (!el->isDone()) { continue; }
			el->invokeCallback();
			gWorker->debug && printf("%s\n", "callback invoked");
			gWorker->tasks.remove(el);

			it++;
		}
		*/
	}
};

void WrapGearmanWorker::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew<String>("WrapGearmanWorker"));

	NODE_SET_PROTOTYPE_METHOD(t, "addServer", addServer);
	NODE_SET_PROTOTYPE_METHOD(t, "addFunction", addFunction);
	NODE_SET_PROTOTYPE_METHOD(t, "start", start);

	NanAssignPersistent(constructor, t->GetFunction());
	exports->Set(NanNew("WrapGearmanWorker"), t->GetFunction());
}

NAN_METHOD(WrapGearmanWorker::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		return NanThrowTypeError("Use the new operator to create new WrapGearmanClient objects");
	}

	WrapGearmanWorker* gWorker = new WrapGearmanWorker();
	gWorker->Wrap(args.This());

	NanReturnValue(args.This());
}

WrapGearmanWorker::WrapGearmanWorker() {
    worker = gearman_worker_create(NULL);

    gearman_worker_set_timeout(worker, 100);

    debug = false;
}

NAN_METHOD(WrapGearmanWorker::addServer) {
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

	WrapGearmanWorker* gWorker = ObjectWrap::Unwrap<WrapGearmanWorker>(args.This());

	gWorker->debug && printf("%s %s %d\n", "addServer", *str, port);
	gearman_return_t ret = gearman_worker_add_server(gWorker->worker, host, (in_port_t) port);

	NanReturnValue(NanNew<Boolean>(gearman_success(ret)));
}

NAN_METHOD(WrapGearmanWorker::addFunction) {
	NanScope();

	if (args.Length() <= 0 || !args[0]->IsString()) {
		return NanThrowTypeError("Argument 0 must be a string");
	}
	String::Utf8Value str(args[0]->ToString());

	if (args.Length() <= 1 || !args[1]->IsFunction()) {
		return NanThrowTypeError("Argument 1 must be a function");
	}
	NanCallback *callback = new NanCallback(args[1].As<Function>());

	WrapGearmanWorker* gWorker = ObjectWrap::Unwrap<WrapGearmanWorker>(args.This());


	char* queue = new char[str.length()];
	strcpy(queue, *str);
	gearman_return_t ret = gearman_worker_register(gWorker->worker, queue, 0);

	NanReturnValue(NanNew<Boolean>(gearman_success(ret)));
}

NAN_METHOD(WrapGearmanWorker::start) {
	NanScope();

	WrapGearmanWorker* gWorker = ObjectWrap::Unwrap<WrapGearmanWorker>(args.This());
	NanAsyncQueueWorker(new GrabJobWorker(gWorker));

	NanReturnValue(args.This());
}

NAN_METHOD(WrapGearmanWorker::stop) {
	NanScope();

	WrapGearmanWorker* gWorker = ObjectWrap::Unwrap<WrapGearmanWorker>(args.This());
	gWorker->endCallback = new NanCallback(args[0].As<Function>());
	gWorker->running = false;

	NanReturnValue(args.This());
}
