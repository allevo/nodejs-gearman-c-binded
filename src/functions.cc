
#include "functions.h"


NAN_METHOD(strerror) {
	NanScope();
	if (args.Length() < 1 || !args[0]->IsInt32()) {
		return NanThrowTypeError("Wrong arguments");
	}
	NanReturnValue(NanNew<String>(gearman_strerror((gearman_return_t) args[0]->Int32Value())));
}

NAN_METHOD(gearmanSuccess) {
	NanScope();
	if (args.Length() < 1 || !args[0]->IsInt32()) {
		return NanThrowTypeError("Wrong arguments");
	}
	NanReturnValue(NanNew<Boolean>(gearman_success((gearman_return_t) args[0]->Int32Value())));
}

NAN_METHOD(gearmanFailed) {
	NanScope();
	if (args.Length() < 1 || !args[0]->IsInt32()) {
		return NanThrowTypeError("Wrong arguments");
	}
	NanReturnValue(NanNew<Boolean>(gearman_failed((gearman_return_t) args[0]->Int32Value())));
}

NAN_METHOD(gearmanContinue) {
	NanScope();
	if (args.Length() < 1 || !args[0]->IsInt32()) {
		return NanThrowTypeError("Wrong arguments");
	}
	NanReturnValue(NanNew<Boolean>(gearman_continue((gearman_return_t) args[0]->Int32Value())));
}
