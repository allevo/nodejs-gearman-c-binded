#ifndef __GEARMANTASK_H__
#define __GEARMANTASK_H__


#include <node.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>


#include <set>
using namespace std;


class GearmanTask : ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static Handle<Value> New(const Arguments& args);

	explicit GearmanTask();
	~GearmanTask() {}

	static Handle<Value> getJobHandle(const Arguments& args);
	static Handle<Value> onSubmitted(const Arguments& args);

	void _onSubmitted();

	GearmanClient* client;

	gearman_return_t ret;
	gearman_job_handle_t job_handle;
	char* data;
	char* unique;
	char* queue;




	Persistent<Function> on_submitted_callback;
};

#endif
