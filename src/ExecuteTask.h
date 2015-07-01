#ifndef __GEARMAN_EXECUTE_TASK_H__
#define __GEARMAN_EXECUTE_TASK_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

#include "WrapGearmanClient.h"
#include "GearmanTask.h"
#include <list>
using namespace std;


class ExecuteTask : public NanAsyncWorker {
private:
	list<GearmanTask*>* tasks;
	WrapGearmanClient* gClient;

public:

	ExecuteTask(WrapGearmanClient* _gClient, list<GearmanTask*>* _tasks, NanCallback* _callback)
		: NanAsyncWorker(_callback), tasks(_tasks), gClient(_gClient) {
	}

	~ExecuteTask() {
		gClient->debug && printf("%s\n", "~ExecuteTask");
	}

	void Execute () {
		gClient->lockClient();
		for(std::list<GearmanTask*>::iterator it = tasks->begin(); it != tasks->end(); it++) {
			GearmanTask* el = *it;
			el->ret = gearman_client_do_background(gClient->client, el->queue, el->unique, el->data, strlen(el->data), el->handle);
		}
		gClient->unlockClient();
	}

	void HandleOKCallback () {
		NanScope();

		Local<Value> argv[1] = { NanNew<Number>(0) };
				printf("%s\n", "GGGGG");
		callback->Call(1, argv);
				printf("%s\n", "HHHHH");
		delete tasks;
	}
};

#endif
