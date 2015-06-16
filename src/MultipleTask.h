#ifndef __GEARMANMULTIPLETASK_H__
#define __GEARMANMULTIPLETASK_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

class GearmanClient;


#include <vector>
using std::vector;

class MultipleTaskElement {
public:
	char* queue;
	char* data;
	char* unique;
	gearman_job_handle_t handle;
	gearman_return_t ret;
};

class MultipleTask : public ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);
	static v8::Local<v8::Object> NewInstance();

	static NAN_METHOD(addJobBackground);

private:
	vector<MultipleTaskElement*> elements;
};

#endif
