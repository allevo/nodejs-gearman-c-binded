#ifndef __GEARMANCLIENT_H__
#define __GEARMANCLIENT_H__


#include <node.h>
using namespace v8;

#include <libgearman/gearman.h>


#include <set>
using namespace std;


class GearmanClient: public node::ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static Handle<Value> New(const Arguments& args);

	static Handle<Value> addServer(const Arguments& args);


	static Handle<Value> doJobBackgroundSync(const Arguments& args);
	static Handle<Value> doJobBackground(const Arguments& args);


	explicit GearmanClient();
	~GearmanClient() {}

	gearman_client_st * client;

	static void queue_job(uv_work_t *work);
	static void after_queue_job(uv_work_t *work, int status);


	// uv_mutex_t client_mutex;
};


#endif