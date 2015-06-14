#ifndef __GEARMANCLIENT_H__
#define __GEARMANCLIENT_H__


#include <node.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>


#include <set>
using namespace std;

class GearmanClient : ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static Handle<Value> New(const Arguments& args);

	static Handle<Value> addServer(const Arguments& args);

	static Handle<Value> doJobBackground(const Arguments& args);
	static Handle<Value> setDebug(const Arguments& args);

	gearman_return_t __doJobBackground(gearman_client_st* client, char* queue, char* id, char* data, gearman_job_handle_t handle);



	explicit GearmanClient();
	~GearmanClient() {}

	gearman_client_st * client;

	static void queue_job(uv_work_t *work);
	static void after_queue_job(uv_work_t *work, int status);

	bool debug;


	// uv_mutex_t client_mutex;
};


#endif