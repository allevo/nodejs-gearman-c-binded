#ifndef __GEARMAN_WRAP_CLIENT_H__
#define __GEARMAN_WRAP_CLIENT_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>


#include <list>
using namespace std;

class Baton;

class WrapGearmanClient : public ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);

	static NAN_METHOD(addServer);

	static NAN_METHOD(setDebug);
	static NAN_METHOD(doBackground);
	static NAN_METHOD(stop);
	static NAN_METHOD(getStatus);

	explicit WrapGearmanClient();
	~WrapGearmanClient() {
		gearman_client_task_free_all(client);
		gearman_client_free(client);
	}

	void lockClient();
	void unlockClient();

	gearman_client_st* client;

	bool debug;
	bool running;

	uv_mutex_t client_mutex;

	list<Baton*> tasks;

	NanCallback* endCallback;
private:
};

#endif
