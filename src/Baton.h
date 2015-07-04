#ifndef __GEARMAN_BATON_H__
#define __GEARMAN_BATON_H__

#include <nan.h>

#include "BackgroundTask.h"
#include "WrapGearmanClient.h"

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

	void invokeCallback() {
		Local<Value> argv[0] = { };
		callback->Call(0, argv);
	}

protected:
	explicit Baton(NanCallback* callback_)
		: callback(callback_), done(false) { }

	~Baton() {
		delete callback;
	}

private:
	NanCallback* callback;
	bool done;
};


class JobBackgroundBaton : public Baton {
public:

	explicit JobBackgroundBaton(BackgroundTask* task_, NanCallback* callback_)
		: Baton(callback_), task(task_) { }

	~JobBackgroundBaton() {
		delete task;
	}

	void Execute(WrapGearmanClient* client);

private:
	BackgroundTask* task;
};

#endif
