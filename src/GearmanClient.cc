

#include "GearmanClient.h"
#include "GearmanTask.h"

#include <string>
#include <string.h>
#include <unistd.h>


Persistent<Function> GearmanClient::constructor;


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
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("GearmanClient"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("doJobBackground"), FunctionTemplate::New(doJobBackground)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("addServer"), FunctionTemplate::New(addServer)->GetFunction());

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
	tpl->Set("GEARMAN_INVALID_SERVER_OPTION", Number::New(GEARMAN_INVALID_SERVER_OPTION));
	tpl->Set("GEARMAN_MAX_RETURN", Number::New(GEARMAN_MAX_RETURN));
	tpl->Set("GEARMAN_FAIL", Number::New(GEARMAN_FAIL));
	tpl->Set("GEARMAN_FATAL", Number::New(GEARMAN_FATAL));
	tpl->Set("GEARMAN_ERROR", Number::New(GEARMAN_ERROR));


	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("GearmanClient"), constructor);
}

Handle<Value> GearmanClient::New(const Arguments& args) {
	HandleScope scope;

	if (args.IsConstructCall()) {
		GearmanClient* obj = new GearmanClient();
		obj->Wrap(args.
			This());
		return args.This();
	} else {
		const int argc = 0;
		Local<Value> argv[argc] = { };
		return scope.Close(constructor->NewInstance(argc, argv));
	}
}

Handle<Value> GearmanClient::addServer(const Arguments& args) {
	HandleScope scope;

	GearmanClient* gClient = ObjectWrap::Unwrap<GearmanClient>(args.This());

	if (!args[0]->IsString()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Boolean::New(false));
	}
	if (!args[1]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Boolean::New(false));
	}

	char* hostname = (char*) _MakeString(args[0]->ToString()).c_str();
	int port = Int32::Cast(*(args[1]))->Value();
	
	char* host = (char*) malloc(sizeof(char) * (strlen(hostname) + 1));
	strcpy(host, hostname);
	host[strlen(hostname)] = '\0';
	gClient->debug && printf("%s %s %d %d\n", "Add server", host, (int) strlen(host), port);

	gearman_return_t ret = gearman_client_add_server(gClient->client, host, port);
	if (gearman_failed(ret)) {
		gClient->debug && printf("ERROR: %s\n", gearman_strerror(ret));
		return scope.Close(Boolean::New(false));
	}
	return scope.Close(Boolean::New(true));
}

Handle<Value> GearmanClient::setDebug(const Arguments& args) {
	HandleScope scope;


	if (!args[0]->IsBoolean()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Boolean::New(false));
	}

	GearmanClient* gClient = ObjectWrap::Unwrap<GearmanClient>(args.This());
	gClient->debug = Boolean::Cast(args[0]);

	return scope.Close(Boolean::New(true));
}


gearman_return_t GearmanClient::__doJobBackground(gearman_client_st* client, char* queue, char* id, char* data, gearman_job_handle_t handle) {
	this->debug && printf("%s %s\n", "QUEUING on ", queue);
	gearman_return_t ret = gearman_client_do_background(client, queue, id, data, strlen(data),  handle);
	this->debug && printf("END queueing%s\n", handle);

	if (!gearman_success(ret)) {
		this->debug && printf("%s %s\n", "ERROR!!", gearman_strerror(ret));
	}

	return ret;
}

struct GearmanJobBaton {
	uv_work_t work;
	char* queue;
	char* id;
	char* data;
	gearman_job_handle_t handler;
	gearman_return_t ret;
	GearmanClient* client;
};


struct TaskBaton {
	// wrap around task
	Persistent<Object> t;
	uv_work_t work;
};

void GearmanClient::queue_job(uv_work_t *work) {
	TaskBaton* baton = (TaskBaton*) work->data;

	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(baton->t);
	gTask->client->debug && printf("%s\n", "Locking...");
	uv_mutex_lock(&(client_mutex));
	gTask->client->debug && printf("%s\n", "DOING JOB BACKGOURND");
	gTask->ret = gTask->client->__doJobBackground(gTask->client->client, (char*) "queue", NULL, (char*) "data", gTask->job_handle);
	gTask->client->debug && printf("%s\n", "Unocking...");
	uv_mutex_unlock(&(client_mutex));
	gTask->client->debug && printf("%s\n", "DONE");
}

void GearmanClient::after_queue_job(uv_work_t *work, int status) {
	HandleScope scope;

	TaskBaton* baton = (TaskBaton*) work->data;
	GearmanTask* gTask = ObjectWrap::Unwrap<GearmanTask>(baton->t);

	gTask->client->debug && printf("%s\n", "argv");
	Handle<Value> argv[2] = {
		Number::New((int) gTask->ret),
		String::New(gTask->job_handle)
	};
	gTask->client->debug && printf("%s\n", "CALLING callback");
	gTask->on_submitted_callback->Call(Context::GetCurrent()->Global(), 2, argv);
	gTask->client->debug && printf("%s\n", "CALLED");


	baton->t.Dispose();
	baton->t.Clear();

	delete baton;
	scope.Close(Undefined());
}

Handle<Value> GearmanClient::doJobBackground(const Arguments& args) {
	HandleScope scope;


	if (!args[0]->IsFunction()) {
		scope.Close(Undefined());
        return ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    }
    
	GearmanClient* gClient = ObjectWrap::Unwrap<GearmanClient>(args.This());
	
	TaskBaton* baton = new TaskBaton();
	baton->t = Persistent<Object>::New(GearmanTask::constructor->NewInstance());
	GearmanTask* task = ObjectWrap::Unwrap<GearmanTask>(baton->t);

	task->data = (char*) "data";
	task->unique = NULL;
	task->queue = (char*) "queue";
	task->client = gClient;

	task->on_submitted_callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
	gClient->debug && printf("%s\n", "CALLBACK SET");

	baton->work.data = (void*) baton;
	uv_queue_work(uv_default_loop(), &(baton->work), GearmanClient::queue_job, GearmanClient::after_queue_job);

	return scope.Close(Undefined());
}
