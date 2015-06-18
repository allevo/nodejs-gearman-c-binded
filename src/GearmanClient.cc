

#include "GearmanClient.h"
#include "ExecuteTask.h"
#include "MultipleTask.h"

#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdio>
using namespace std;



Persistent<Function> GearmanClient::constructor;


uv_mutex_t client_mutex;


GearmanClient::GearmanClient() {
	client = gearman_client_create(NULL);
	uv_mutex_init(&client_mutex);
	debug = false;
}

void GearmanClient::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew<String>("GearmanClient"));

	NODE_SET_PROTOTYPE_METHOD(t, "doJobBackground", doJobBackground);
	NODE_SET_PROTOTYPE_METHOD(t, "setDebug", setDebug);
	NODE_SET_PROTOTYPE_METHOD(t, "addServer", addServer);
	NODE_SET_PROTOTYPE_METHOD(t, "getMultipleTask", getMultipleTask);

	NanAssignPersistent(constructor, t->GetFunction());
	exports->Set(NanNew("GearmanClient"), t->GetFunction());
}

NAN_METHOD(GearmanClient::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		return NanThrowTypeError("Use the new operator to create new GearmanClient objects");
	}

	GearmanClient* gClient = new GearmanClient();
	gClient->Wrap(args.This());

	NanReturnValue(args.This());
}

NAN_METHOD(GearmanClient::addServer) {
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

	GearmanClient* gClient = ObjectWrap::Unwrap<GearmanClient>(args.This());

	gClient->debug && printf("%s %s %d\n", "addServer", *str, port);
	gearman_return_t ret = gearman_client_add_server(gClient->client, host, (in_port_t) port);

	NanReturnValue(NanNew<Boolean>(gearman_success(ret)));
}

NAN_METHOD(GearmanClient::setDebug) {
	NanScope();

	GearmanClient* gClient = ObjectWrap::Unwrap<GearmanClient>(args.This());
	if (args.Length() == 1) {
		gClient->debug = args[1]->BooleanValue();
	}

	NanReturnValue(NanNew<Boolean>(true));
}

NAN_METHOD(GearmanClient::doJobBackground) {
	NanScope();

	if (args.Length() <= 0 || !args[0]->IsString()) {
		return NanThrowTypeError("Argument 0 must be a string");
	}
	String::Utf8Value queue(args[0]->ToString());

	if (args.Length() <= 1 || !args[1]->IsString()) {
		return NanThrowTypeError("Argument 1 must be a string");
	}
	String::Utf8Value data(args[1]->ToString());

	if (args.Length() <= 2 || !args[2]->IsString()) {
		return NanThrowTypeError("Argument 2 must be a string");
	}
	String::Utf8Value unique(args[2]->ToString());

	if (args.Length() <= 3 || !args[3]->IsFunction()) {
		return NanThrowTypeError("Argument 3 must be a function");
	}
	NanCallback *callback = new NanCallback(args[3].As<Function>());

	GearmanClient* gClient = ObjectWrap::Unwrap<GearmanClient>(args.This());

	NanAsyncQueueWorker(new ExecuteTask(gClient, callback, *queue, *data, *unique));

	NanReturnUndefined();
}

NAN_METHOD(GearmanClient::getMultipleTask) {
	NanScope();

	NanReturnValue(MultipleTask::NewInstance());
}

