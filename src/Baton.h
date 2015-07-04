#ifndef __GEARMAN_BATON_H__
#define __GEARMAN_BATON_H__

#include <nan.h>

#include "BackgroundTask.h"
#include "WrapGearmanClient.h"

#include <libgearman/gearman.h>

class WrapGearmanClient;

class Baton {

public:
	virtual void Execute(WrapGearmanClient* client) = 0;

	bool isDone() {
		return done;
	}

	void setToDone() {
		done = true;
	}

	virtual void invokeCallback() {
		Local<Value> argv[0] = { };
		callback->Call(0, argv);
	}

protected:
	explicit Baton(NanCallback* callback_)
		: callback(callback_), done(false) { }

	~Baton() {
		delete callback;
	}

	NanCallback* callback;

private:
	bool done;
};

class JobBackgroundBaton : public Baton {
public:

	explicit JobBackgroundBaton(BackgroundTask* task_, NanCallback* callback_)
		: Baton(callback_), task(task_) { }

	~JobBackgroundBaton() {
		delete task;
	}

	virtual void Execute(WrapGearmanClient* client);

private:
	BackgroundTask* task;
};

class JobStatusBaton : public Baton {
public:

	explicit JobStatusBaton(char* handle_, NanCallback* callback_)
		: Baton(callback_) {

		handle = new char[GEARMAN_JOB_HANDLE_SIZE];
		strcpy(handle, handle_);
	}

	~JobStatusBaton() {
		delete handle;
	}

	virtual void Execute(WrapGearmanClient* client);

	virtual void invokeCallback() {
		NanScope();

		Local<Object> result = NanNew<Object>();
		result->Set(NanNew<String>("is_known"), NanNew<v8::Boolean>(is_known));
		result->Set(NanNew<String>("is_running"), NanNew<v8::Boolean>(is_running));
		result->Set(NanNew<String>("numerator"), NanNew<v8::Integer>(numerator));
		result->Set(NanNew<String>("denominator"), NanNew<v8::Integer>(denominator));
		result->Set(NanNew<String>("returnCode"), NanNew<v8::Integer>((int) ret));

		Local<Value> argv[1] = { result };
		callback->Call(1, argv);
	}

private:
	char* handle;
	bool is_known;
	bool is_running;
	uint32_t numerator;
	uint32_t denominator;

	gearman_return_t ret;
};

#endif
