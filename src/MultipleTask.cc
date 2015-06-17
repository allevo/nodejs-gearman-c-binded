
#include "MultipleTask.h"


Persistent<Function> MultipleTask::constructor;


void MultipleTask::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew("MultipleTask"));

	NODE_SET_PROTOTYPE_METHOD(t, "addJobBackground", addJobBackground);

	NanAssignPersistent(constructor, t->GetFunction());
	exports->Set(NanNew("MultipleTask"), t->GetFunction());
}

NAN_METHOD(MultipleTask::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		return NanThrowTypeError("Use the new operator to create new MultipleTask objects");
	}

	MultipleTask* gClient = new MultipleTask();
	gClient->Wrap(args.This());

	NanReturnValue(args.This());
}

Local<Object> MultipleTask::NewInstance() {
	NanEscapableScope();

	Local<Value> argv[0] = { };
	Local<Function> cons = NanNew<Function>(MultipleTask::constructor);
	Local<Object> instance = cons->NewInstance(0, argv);

	return NanEscapeScope(instance);
}


NAN_METHOD(MultipleTask::addJobBackground) {
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

	if (args.Length() <= 3 || !args[3]->IsString()) {
		return NanThrowTypeError("Argument 3 must be a string");
	}
	String::Utf8Value name(args[2]->ToString());

	MultipleTask* mTask = ObjectWrap::Unwrap<MultipleTask>(args.This());

	MultipleTaskElement* element = new MultipleTaskElement();
	element->queue = new char[queue.length()];
	strcpy(element->queue, *queue);
	element->data = new char[data.length()];
	strcpy(element->data, *data);
	element->unique = new char[unique.length()];
	strcpy(element->unique, *unique);

	mTask->elements.push_back(element);

	NanReturnValue(args.This());
}
