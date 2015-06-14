
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif


#include "GearmanClient.h"
#include "GearmanWorker.h"
#include "GearmanTask.h"

#include <string>
#include <node.h>
#include <string.h>

#include <libgearman/gearman.h>


#include <unistd.h>
#include <stdlib.h>

#include <set>


using namespace std;
using namespace node;
using namespace v8;

struct Baton {
	Persistent<Function> callback;
	uv_work_t* work;
};

void _sleep(uv_work_t *work) {
	Baton* baton = (Baton*) work->data;

	usleep(1000 * 1000 * 0.5); // 0.5 sec
}

void _after_sleep(uv_work_t *work, int status) {
	Baton* baton = (Baton*) work->data;

	Handle<Value> argv[0] = {};
	baton->callback->Call(Context::GetCurrent()->Global(), 0, argv);
}

Handle<Value> Run(const Arguments& args) {
	HandleScope scope;

	Baton* baton = new Baton();
	baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));
	baton->work = (uv_work_t*) malloc(sizeof(uv_work_t) * 1);
	baton->work->data = baton;
	uv_queue_work(uv_default_loop(),
		baton->work,
		_sleep,
		_after_sleep);

	return scope.Close(Undefined());
}


void InitAll(Handle<Object> exports, Handle<Object> module) {
	exports->Set(String::NewSymbol("run"),
      FunctionTemplate::New(Run)->GetFunction());

	GearmanClient::Init(exports);
	GearmanWorker::Init(exports);
	GearmanTask::Init(exports);
}

NODE_MODULE(gearmannodeCBinded, InitAll)
