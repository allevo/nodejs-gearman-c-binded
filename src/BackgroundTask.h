#ifndef __GEARMAN_BACKGROUND_TASK_H__
#define __GEARMAN_BACKGROUND_TASK_H__


#include <v8.h>
#include <node.h>
#include <nan.h>
using namespace node;
using namespace v8;

#include <libgearman/gearman.h>


class BackgroundTask: public ObjectWrap {
public:
	static Persistent<Function> constructor;

	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);

	static NAN_METHOD(getHandle);
	static NAN_METHOD(getUnique);
	static NAN_METHOD(getReturnCode);

	explicit BackgroundTask(char* queue, char* data, char* unique);
	~BackgroundTask();

	char* queue;
	char* data;
	char* unique;
	char* handle;
	gearman_return_t ret;

	bool done;

	NanCallback *callback;

private:
};

#endif
