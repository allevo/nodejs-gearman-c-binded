#ifndef __GEARMAN_WRAP_CLIENT_H__
#define __GEARMAN_WRAP_CLIENT_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

class WrapGearmanClient : public ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);

	static NAN_METHOD(addServer);

	static NAN_METHOD(setDebug);
	static NAN_METHOD(_execute);

	explicit WrapGearmanClient();
	~WrapGearmanClient() {}

	gearman_client_st* client;

	bool debug;
};

#endif
