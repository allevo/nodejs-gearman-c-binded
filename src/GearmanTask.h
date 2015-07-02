#ifndef __GEARMAN_TASK_H__
#define __GEARMAN_TASK_H__


#include <v8.h>
#include <node.h>
#include <nan.h>
using namespace node;
using namespace v8;

#include <libgearman/gearman.h>


class GearmanTask: public ObjectWrap {
public:
	static Persistent<Function> constructor;

	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);

	static NAN_METHOD(getHandle);
	static NAN_METHOD(getUnique);
	static NAN_METHOD(getReturnCode);

	explicit GearmanTask(char* queue, char* data, char* unique);
	~GearmanTask();

	char* queue;
	char* data;
	char* unique;
	gearman_job_handle_t handle;
	gearman_return_t ret;

	bool done;

	NanCallback *callback;

private:
};

#endif
