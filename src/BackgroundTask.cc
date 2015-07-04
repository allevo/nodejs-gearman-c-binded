
#include "BackgroundTask.h"


Persistent<Function> BackgroundTask::constructor;

void BackgroundTask::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew<String>("BackgroundTask"));

	NODE_SET_PROTOTYPE_METHOD(t, "handle", getHandle);
	NODE_SET_PROTOTYPE_METHOD(t, "unique", getUnique);
	NODE_SET_PROTOTYPE_METHOD(t, "returnCode", getReturnCode);

	NanAssignPersistent(constructor, t->GetFunction());
	exports->Set(NanNew("BackgroundTask"), t->GetFunction());
}

NAN_METHOD(BackgroundTask::getHandle) {
	NanScope();
	BackgroundTask* gTask = ObjectWrap::Unwrap<BackgroundTask>(args.This());

	NanReturnValue(NanNew<String>(gTask->handle));
}

NAN_METHOD(BackgroundTask::getUnique) {
	NanScope();
	BackgroundTask* gTask = ObjectWrap::Unwrap<BackgroundTask>(args.This());

	NanReturnValue(NanNew<String>(gTask->unique));
}

NAN_METHOD(BackgroundTask::getReturnCode) {
	NanScope();
	BackgroundTask* gTask = ObjectWrap::Unwrap<BackgroundTask>(args.This());

	NanReturnValue(NanNew<Number>((int) gTask->ret));
}

NAN_METHOD(BackgroundTask::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		return NanThrowTypeError("Use the new operator to create new BackgroundTask objects");
	}

	if (args.Length() <= 0 || !args[0]->IsString()) {
		return NanThrowTypeError("Argument 0 must be a string");
	}
	String::Utf8Value queue(args[0]->ToString());

	if (args.Length() <= 1 || !args[1]->IsString()) {
		return NanThrowTypeError("Argument 1 must be a string");
	}
	String::Utf8Value data(args[1]->ToString());

	if (args.Length() <= 2 || !(args[2]->IsString() || args[2]->IsNull())) {
		return NanThrowTypeError("Argument 2 must be a string");
	}
	String::Utf8Value unique(args[2]->ToString());

	BackgroundTask* gTask = new BackgroundTask(*queue, *data, args[2]->IsNull() ? NULL : *unique);
	gTask->done = false;
	gTask->Wrap(args.This());

	NanReturnValue(args.This());
}

BackgroundTask::BackgroundTask(char* queue, char* data, char* unique) {
	this->queue = new char[strlen(queue)];
	strcpy(this->queue, queue);
	this->data = new char[strlen(data)];
	strcpy(this->data, data);

	this->unique = NULL;
	if (unique != NULL) {
		this->unique = new char[strlen(unique)];
		strcpy(this->unique, unique);
	}
}

BackgroundTask::~BackgroundTask() {
	printf("%s\n", "~BackgroundTask");
	delete this->queue;
	delete this->data;
	delete this->unique;
}
