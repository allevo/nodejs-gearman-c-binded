#ifndef __GEARMAN_WRAP_WORKER_H__
#define __GEARMAN_WRAP_WORKER_H__

#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>


#include <list>
using namespace std;


class GearmanJob {
public:

	GearmanJob() {
		ret = (gearman_return_t*) malloc(sizeof(gearman_return_t) * 1);
	}

	gearman_job_st* job;
	gearman_return_t* ret;
};

class WrapGearmanWorker : public ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);

	static NAN_METHOD(addServer);
	static NAN_METHOD(addFunction);
	static NAN_METHOD(start);
	static NAN_METHOD(stop);

	explicit WrapGearmanWorker();
	~WrapGearmanWorker() {
		gearman_worker_free(worker);
	}

	bool debug;
	bool running;

	NanCallback* endCallback;

	list<GearmanJob*> jobs;

	gearman_worker_st* worker;
};

#endif
