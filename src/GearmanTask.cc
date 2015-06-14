

#include "GearmanTask.h"


Persistent<Function> GearmanTask::constructor;

GearmanTask::GearmanTask() { }

void GearmanTask::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("GearmanTask"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("getJobHandle"), FunctionTemplate::New(getJobHandle)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("onSubmitted"), FunctionTemplate::New(onSubmitted)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("GearmanTask"), constructor);
}


Handle<Value> GearmanTask::New(const Arguments& args) {
	HandleScope scope;

	if (args.IsConstructCall()) {
		GearmanTask* obj = new GearmanTask();
		obj->Wrap(args.This());
		return args.This();
	} else {
		const int argc = 0;
		Local<Value> argv[argc] = { };
		return scope.Close(constructor->NewInstance(argc, argv));
	}
}

Handle<Value> GearmanTask::getJobHandle(const Arguments& args) {
	HandleScope scope;

	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(args.This());

	return scope.Close(String::New(gTask->job_handle));
}

Handle<Value> GearmanTask::onSubmitted(const Arguments& args) {
	HandleScope scope;

	if (!args[0]->IsFunction()) {
		scope.Close(Undefined());
        return ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    }
    
	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(args.This());

	gTask->on_submitted_callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));

	return scope.Close(String::New(gTask->job_handle));
}

void GearmanTask::_onSubmitted() {
	printf("CALLING ON SUBMITTED CALLBACK %p\n", Context::GetCurrent()->Global());
	Handle<Value> argv[0] = {};

	this->on_submitted_callback->Call(Context::GetCurrent()->Global(), 0, argv);
}
