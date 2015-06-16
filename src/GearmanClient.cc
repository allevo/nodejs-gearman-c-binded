

#include "GearmanClient.h"
#include "ExecuteTask.h"

#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdio>
using namespace std;



Persistent<FunctionTemplate> GearmanClient::constructor;


uv_mutex_t client_mutex;


std::string _MakeString(Handle<String> str) {
	String::Utf8Value s(str);
	std::string ss(*s);
	return ss;
}

GearmanClient::GearmanClient() {
	client = gearman_client_create(NULL);
	uv_mutex_init(&client_mutex);
	debug = false;
}

void GearmanClient::Init(Handle<Object> exports) {
	NanScope();
	Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(NanNew("GearmanClient"));
	NODE_SET_PROTOTYPE_METHOD(t, "doJobBackground", doJobBackground);
	NODE_SET_PROTOTYPE_METHOD(t, "setDebug", setDebug);
	NODE_SET_PROTOTYPE_METHOD(t, "addServer", addServer);

	NanAssignPersistent(constructor, t);
	exports->Set(NanNew("GearmanClient"), t->GetFunction());

	/*
	tpl->Set("GEARMAN_SUCCESS", Number::New(GEARMAN_SUCCESS));
	tpl->Set("GEARMAN_IO_WAIT", Number::New(GEARMAN_IO_WAIT));
	tpl->Set("GEARMAN_SHUTDOWN", Number::New(GEARMAN_SHUTDOWN));
	tpl->Set("GEARMAN_SHUTDOWN_GRACEFUL", Number::New(GEARMAN_SHUTDOWN_GRACEFUL));
	tpl->Set("GEARMAN_ERRNO", Number::New(GEARMAN_ERRNO));
	tpl->Set("GEARMAN_EVENT", Number::New(GEARMAN_EVENT));
	tpl->Set("GEARMAN_TOO_MANY_ARGS", Number::New(GEARMAN_TOO_MANY_ARGS));
	tpl->Set("GEARMAN_NO_ACTIVE_FDS", Number::New(GEARMAN_NO_ACTIVE_FDS));
	tpl->Set("GEARMAN_INVALID_MAGIC", Number::New(GEARMAN_INVALID_MAGIC));
	tpl->Set("GEARMAN_INVALID_COMMAND", Number::New(GEARMAN_INVALID_COMMAND));
	tpl->Set("GEARMAN_INVALID_PACKET", Number::New(GEARMAN_INVALID_PACKET));
	tpl->Set("GEARMAN_UNEXPECTED_PACKET", Number::New(GEARMAN_UNEXPECTED_PACKET));
	tpl->Set("GEARMAN_GETADDRINFO", Number::New(GEARMAN_GETADDRINFO));
	tpl->Set("GEARMAN_NO_SERVERS", Number::New(GEARMAN_NO_SERVERS));
	tpl->Set("GEARMAN_LOST_CONNECTION", Number::New(GEARMAN_LOST_CONNECTION));
	tpl->Set("GEARMAN_MEMORY_ALLOCATION_FAILURE", Number::New(GEARMAN_MEMORY_ALLOCATION_FAILURE));
	tpl->Set("GEARMAN_JOB_EXISTS", Number::New(GEARMAN_JOB_EXISTS));
	tpl->Set("GEARMAN_JOB_QUEUE_FULL", Number::New(GEARMAN_JOB_QUEUE_FULL));
	tpl->Set("GEARMAN_SERVER_ERROR", Number::New(GEARMAN_SERVER_ERROR));
	tpl->Set("GEARMAN_WORK_ERROR", Number::New(GEARMAN_WORK_ERROR));
	tpl->Set("GEARMAN_WORK_DATA", Number::New(GEARMAN_WORK_DATA));
	tpl->Set("GEARMAN_WORK_WARNING", Number::New(GEARMAN_WORK_WARNING));
	tpl->Set("GEARMAN_WORK_STATUS", Number::New(GEARMAN_WORK_STATUS));
	tpl->Set("GEARMAN_WORK_EXCEPTION", Number::New(GEARMAN_WORK_EXCEPTION));
	tpl->Set("GEARMAN_WORK_FAIL", Number::New(GEARMAN_WORK_FAIL));
	tpl->Set("GEARMAN_NOT_CONNECTED", Number::New(GEARMAN_NOT_CONNECTED));
	tpl->Set("GEARMAN_COULD_NOT_CONNECT", Number::New(GEARMAN_COULD_NOT_CONNECT));
	tpl->Set("GEARMAN_SEND_IN_PROGRESS", Number::New(GEARMAN_SEND_IN_PROGRESS));
	tpl->Set("GEARMAN_RECV_IN_PROGRESS", Number::New(GEARMAN_RECV_IN_PROGRESS));
	tpl->Set("GEARMAN_NOT_FLUSHING", Number::New(GEARMAN_NOT_FLUSHING));
	tpl->Set("GEARMAN_DATA_TOO_LARGE", Number::New(GEARMAN_DATA_TOO_LARGE));
	tpl->Set("GEARMAN_INVALID_FUNCTION_NAME", Number::New(GEARMAN_INVALID_FUNCTION_NAME));
	tpl->Set("GEARMAN_INVALID_WORKER_FUNCTION", Number::New(GEARMAN_INVALID_WORKER_FUNCTION));
	tpl->Set("GEARMAN_NO_REGISTERED_FUNCTION", Number::New(GEARMAN_NO_REGISTERED_FUNCTION));
	tpl->Set("GEARMAN_NO_REGISTERED_FUNCTIONS", Number::New(GEARMAN_NO_REGISTERED_FUNCTIONS));
	tpl->Set("GEARMAN_NO_JOBS", Number::New(GEARMAN_NO_JOBS));
	tpl->Set("GEARMAN_ECHO_DATA_CORRUPTION", Number::New(GEARMAN_ECHO_DATA_CORRUPTION));
	tpl->Set("GEARMAN_NEED_WORKLOAD_FN", Number::New(GEARMAN_NEED_WORKLOAD_FN));
	tpl->Set("GEARMAN_PAUSE", Number::New(GEARMAN_PAUSE));
	tpl->Set("GEARMAN_UNKNOWN_STATE", Number::New(GEARMAN_UNKNOWN_STATE));
	tpl->Set("GEARMAN_PTHREAD", Number::New(GEARMAN_PTHREAD));
	tpl->Set("GEARMAN_PIPE_EOF", Number::New(GEARMAN_PIPE_EOF));
	tpl->Set("GEARMAN_QUEUE_ERROR", Number::New(GEARMAN_QUEUE_ERROR));
	tpl->Set("GEARMAN_FLUSH_DATA", Number::New(GEARMAN_FLUSH_DATA));
	tpl->Set("GEARMAN_SEND_BUFFER_TOO_SMALL", Number::New(GEARMAN_SEND_BUFFER_TOO_SMALL));
	tpl->Set("GEARMAN_IGNORE_PACKET", Number::New(GEARMAN_IGNORE_PACKET));
	tpl->Set("GEARMAN_UNKNOWN_OPTION", Number::New(GEARMAN_UNKNOWN_OPTION));
	tpl->Set("GEARMAN_TIMEOUT", Number::New(GEARMAN_TIMEOUT));
	tpl->Set("GEARMAN_ARGUMENT_TOO_LARGE", Number::New(GEARMAN_ARGUMENT_TOO_LARGE));
	tpl->Set("GEARMAN_INVALID_ARGUMENT", Number::New(GEARMAN_INVALID_ARGUMENT));
	tpl->Set("GEARMAN_IN_PROGRESS", Number::New(GEARMAN_IN_PROGRESS));
#ifdef GEARMAN_INVALID_SERVER_OPTION
	tpl->Set("GEARMAN_INVALID_SERVER_OPTION", Number::New(GEARMAN_INVALID_SERVER_OPTION));
#endif
	tpl->Set("GEARMAN_MAX_RETURN", Number::New(GEARMAN_MAX_RETURN));
#ifdef GEARMAN_FAIL
	tpl->Set("GEARMAN_FAIL", Number::New(GEARMAN_FAIL));
#endif
	tpl->Set("GEARMAN_FATAL", Number::New(GEARMAN_FATAL));
	tpl->Set("GEARMAN_ERROR", Number::New(GEARMAN_ERROR));
	*/
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
    gearman_client_add_server(gClient->client, host, (in_port_t) port);

	NanReturnValue(NanNew<Boolean>(true));
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
