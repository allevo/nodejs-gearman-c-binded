#ifndef __GEARMANCLIENT_H__
#define __GEARMANCLIENT_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

#include <set>
using namespace std;

class GearmanClient : public ObjectWrap {
public:
	static Persistent<Function> constructor;
	static void Init(Handle<Object> exports);
	static NAN_METHOD(New);

	static NAN_METHOD(addServer);

	static NAN_METHOD(doJobBackground);
	static NAN_METHOD(setDebug);
	static NAN_METHOD(getMultipleTask);

	explicit GearmanClient();
	~GearmanClient() {}

	gearman_client_st * client;

	bool debug;
};

#endif
