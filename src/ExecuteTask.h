#ifndef __GEARMANEXECUTETASK_H__
#define __GEARMANEXECUTETASK_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

class GearmanClient;

#include <unistd.h>


class ExecuteTask : public NanAsyncWorker {
public:
	ExecuteTask(GearmanClient* _gClient, NanCallback* _callback, char* _queue, char* _data, char* _unique)
		: NanAsyncWorker(_callback), gClient(_gClient) {
			queue = new char[strlen(_queue)];
			strcpy(queue, _queue);

			data = new char[strlen(_data)];
			strcpy(data, _data);

			unique = new char[strlen(_unique)];
			strcpy(unique, _unique);
		}
	~ExecuteTask() {
		gClient->debug && printf("%s\n", "~ExecuteTask");
		delete queue;
		delete data;
		delete unique;
	}

	void Execute () {
		ret = gearman_client_do_background(gClient->client, queue, unique, data, strlen(data), handle);
		gClient->debug && printf("%s %s %s %s %d %s %s\n", "EXECUTE!!", queue, data, unique, ret, gearman_strerror(ret), handle);
	}

	void HandleOKCallback () {
		NanScope();

		Local<Value> argv[] = { NanNew<Number>(ret) , NanNew<String>(handle) };

		callback->Call(2, argv);
	}

private:
	char* queue;
	char* data;
	char* unique;
	gearman_job_handle_t handle;
	gearman_return_t ret;
	GearmanClient* gClient;
};

#endif
