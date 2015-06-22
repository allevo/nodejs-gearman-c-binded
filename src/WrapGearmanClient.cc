

#include "WrapGearmanClient.h"
#include "ExecuteTask.h"
#include "GearmanTask.h"

#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdio>
using namespace std;



Persistent<Function> WrapGearmanClient::constructor;


uv_mutex_t client_mutex;


WrapGearmanClient::WrapGearmanClient() {
	client = gearman_client_create(NULL);
	printf("gearman_client_create %p\n", client);
	uv_mutex_init(&client_mutex);
	debug = false;
}

void WrapGearmanClient::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew<String>("WrapGearmanClient"));

	NODE_SET_PROTOTYPE_METHOD(t, "setDebug", setDebug);
	NODE_SET_PROTOTYPE_METHOD(t, "addServer", addServer);
	NODE_SET_PROTOTYPE_METHOD(t, "_execute", _execute);

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

NAN_METHOD(WrapGearmanClient::setDebug) {
	NanScope();

	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());
	if (args.Length() == 1) {
		gClient->debug = args[1]->BooleanValue();
	}

	NanReturnValue(NanNew<Boolean>(true));
}


NAN_METHOD(WrapGearmanClient::_execute) {
	NanScope();

	if (args.Length() <= 0 || !args[0]->IsArray()) {
		return NanThrowTypeError("Argument 0 must be an array");
	}

	if (args.Length() <= 1 || !args[1]->IsFunction()) {
		return NanThrowTypeError("Argument 1 must be a function");
	}
	NanCallback *callback = new NanCallback(args[1].As<Function>());
	list<GearmanTask*>* tasks = new list<GearmanTask*>;
	Local<Array> array = Local<Array>::Cast(args[0]);
	int length = array->Length();
	for (int i = 0, pos = 1; i < length; i++, pos++) {
		Local<Value> taskObject = array->Get(i);
		GearmanTask* task = ObjectWrap::Unwrap<GearmanTask>(taskObject->ToObject());
printf("%s %p\n", "AAAAAAAAAA", task);
		tasks->push_back(task);
	}

	WrapGearmanClient* gClient = ObjectWrap::Unwrap<WrapGearmanClient>(args.This());
	printf("%s %p\n", "BEFORE", gClient->client);
	NanAsyncQueueWorker(new ExecuteTask(gClient, tasks, callback));

	NanReturnValue(NanNew<Boolean>(true));
}

