
#include "GearmanTask.h"


Persistent<Function> GearmanTask::constructor;

void GearmanTask::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew<String>("GearmanTask"));

	NODE_SET_PROTOTYPE_METHOD(t, "handle", getHandle);
	NODE_SET_PROTOTYPE_METHOD(t, "unique", getUnique);
	NODE_SET_PROTOTYPE_METHOD(t, "returnCode", getReturnCode);

	NanAssignPersistent(constructor, t->GetFunction());
	exports->Set(NanNew("GearmanTask"), t->GetFunction());
}

NAN_METHOD(GearmanTask::getHandle) {
	NanScope();
	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(args.This());

	NanReturnValue(NanNew<String>(gTask->handle));
}

NAN_METHOD(GearmanTask::getUnique) {
	NanScope();
	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(args.This());

	NanReturnValue(NanNew<String>(gTask->unique));
}

NAN_METHOD(GearmanTask::getReturnCode) {
	NanScope();
	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(args.This());

	NanReturnValue(NanNew<Number>(gTask->ret));
}

NAN_METHOD(GearmanTask::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		return NanThrowTypeError("Use the new operator to create new GearmanTask objects");
	}

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

	GearmanTask* gClient = new GearmanTask(*queue, *data, *unique);
	gClient->Wrap(args.This());

	NanReturnValue(args.This());
}

GearmanTask::GearmanTask(char* queue, char* data, char* unique) {
	this->queue = new char[strlen(queue)];
	strcpy(this->queue, queue);
	this->data = new char[strlen(data)];
	strcpy(this->data, data);
	this->unique = new char[strlen(unique)];
	strcpy(this->unique, unique);
}

GearmanTask::~GearmanTask() {
	printf("%s\n", "~GearmanTask");
	delete this->queue;
	delete this->data;
	delete this->unique;
}
