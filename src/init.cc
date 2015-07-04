
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif


#include "WrapGearmanClient.h"
#include "BackgroundTask.h"
#include "functions.h"

#define DEFINE_CONSTANT(t, c)									\
	t->ForceSet(NanNew<String>(#c),								\
		NanNew<Integer>(c),										\
		static_cast<PropertyAttribute>(ReadOnly | DontDelete)	\
	);

void defineConstants(Handle<Object> exports) {
	DEFINE_CONSTANT(exports, GEARMAN_SUCCESS)
	DEFINE_CONSTANT(exports, GEARMAN_IO_WAIT)
	DEFINE_CONSTANT(exports, GEARMAN_SHUTDOWN)
	DEFINE_CONSTANT(exports, GEARMAN_SHUTDOWN_GRACEFUL)
	DEFINE_CONSTANT(exports, GEARMAN_ERRNO)
	DEFINE_CONSTANT(exports, GEARMAN_EVENT)
	DEFINE_CONSTANT(exports, GEARMAN_TOO_MANY_ARGS)
	DEFINE_CONSTANT(exports, GEARMAN_NO_ACTIVE_FDS)
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_MAGIC)
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_COMMAND)
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_PACKET)
	DEFINE_CONSTANT(exports, GEARMAN_UNEXPECTED_PACKET)
	DEFINE_CONSTANT(exports, GEARMAN_GETADDRINFO)
	DEFINE_CONSTANT(exports, GEARMAN_NO_SERVERS)
	DEFINE_CONSTANT(exports, GEARMAN_LOST_CONNECTION)
	DEFINE_CONSTANT(exports, GEARMAN_MEMORY_ALLOCATION_FAILURE)
	DEFINE_CONSTANT(exports, GEARMAN_JOB_EXISTS)
	DEFINE_CONSTANT(exports, GEARMAN_JOB_QUEUE_FULL)
	DEFINE_CONSTANT(exports, GEARMAN_SERVER_ERROR)
	DEFINE_CONSTANT(exports, GEARMAN_WORK_ERROR)
	DEFINE_CONSTANT(exports, GEARMAN_WORK_DATA)
	DEFINE_CONSTANT(exports, GEARMAN_WORK_WARNING)
	DEFINE_CONSTANT(exports, GEARMAN_WORK_STATUS)
	DEFINE_CONSTANT(exports, GEARMAN_WORK_EXCEPTION)
	DEFINE_CONSTANT(exports, GEARMAN_WORK_FAIL)
	DEFINE_CONSTANT(exports, GEARMAN_NOT_CONNECTED)
	DEFINE_CONSTANT(exports, GEARMAN_COULD_NOT_CONNECT)
	DEFINE_CONSTANT(exports, GEARMAN_SEND_IN_PROGRESS)
	DEFINE_CONSTANT(exports, GEARMAN_RECV_IN_PROGRESS)
	DEFINE_CONSTANT(exports, GEARMAN_NOT_FLUSHING)
	DEFINE_CONSTANT(exports, GEARMAN_DATA_TOO_LARGE)
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_FUNCTION_NAME)
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_WORKER_FUNCTION)
	DEFINE_CONSTANT(exports, GEARMAN_NO_REGISTERED_FUNCTION)
	DEFINE_CONSTANT(exports, GEARMAN_NO_REGISTERED_FUNCTIONS)
	DEFINE_CONSTANT(exports, GEARMAN_NO_JOBS)
	DEFINE_CONSTANT(exports, GEARMAN_ECHO_DATA_CORRUPTION)
	DEFINE_CONSTANT(exports, GEARMAN_NEED_WORKLOAD_FN)
	DEFINE_CONSTANT(exports, GEARMAN_PAUSE)
	DEFINE_CONSTANT(exports, GEARMAN_UNKNOWN_STATE)
	DEFINE_CONSTANT(exports, GEARMAN_PTHREAD)
	DEFINE_CONSTANT(exports, GEARMAN_PIPE_EOF)
	DEFINE_CONSTANT(exports, GEARMAN_QUEUE_ERROR)
	DEFINE_CONSTANT(exports, GEARMAN_FLUSH_DATA)
	DEFINE_CONSTANT(exports, GEARMAN_SEND_BUFFER_TOO_SMALL)
	DEFINE_CONSTANT(exports, GEARMAN_IGNORE_PACKET)
	DEFINE_CONSTANT(exports, GEARMAN_UNKNOWN_OPTION)
	DEFINE_CONSTANT(exports, GEARMAN_TIMEOUT)
	DEFINE_CONSTANT(exports, GEARMAN_ARGUMENT_TOO_LARGE)
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_ARGUMENT)
	DEFINE_CONSTANT(exports, GEARMAN_IN_PROGRESS)
#ifdef GEARMAN_INVALID_SERVER_OPTION
	DEFINE_CONSTANT(exports, GEARMAN_INVALID_SERVER_OPTION)
#endif
	DEFINE_CONSTANT(exports, GEARMAN_MAX_RETURN)
#ifdef GEARMAN_FAIL
	DEFINE_CONSTANT(exports, GEARMAN_FAIL)
#endif
	DEFINE_CONSTANT(exports, GEARMAN_FATAL)
	DEFINE_CONSTANT(exports, GEARMAN_ERROR)

	exports->ForceSet(NanNew<String>("LIBGEARMAN_VERSION_STRING"),
		NanNew<String>(LIBGEARMAN_VERSION_STRING),
		static_cast<PropertyAttribute>(ReadOnly | DontDelete)
	);
	exports->ForceSet(NanNew<String>("LIBGEARMAN_VERSION_HEX"),
		NanNew<Integer>(LIBGEARMAN_VERSION_HEX),
		static_cast<PropertyAttribute>(ReadOnly | DontDelete)
	);
}


void defineFunctions(Handle<Object> exports) {
	exports->Set(NanNew("strerror"), NanNew<FunctionTemplate>(strerror)->GetFunction());
	exports->Set(NanNew("gearmanSuccess"), NanNew<FunctionTemplate>(gearmanSuccess)->GetFunction());
	exports->Set(NanNew("gearmanFailed"), NanNew<FunctionTemplate>(gearmanFailed)->GetFunction());
	exports->Set(NanNew("gearmanContinue"), NanNew<FunctionTemplate>(gearmanContinue)->GetFunction());
}


void InitAll(Handle<Object> exports, Handle<Object> module) {
	WrapGearmanClient::Init(exports);
	BackgroundTask::Init(exports);
	defineConstants(exports);
	defineFunctions(exports);
}

NODE_MODULE(gearmannodeCBinded, InitAll)
