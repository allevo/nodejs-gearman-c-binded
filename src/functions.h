#ifndef __GEARMANFUNCTIONS_H__
#define __GEARMANFUNCTIONS_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

NAN_METHOD(strerror);
NAN_METHOD(gearmanSuccess);
NAN_METHOD(gearmanFailed);
NAN_METHOD(gearmanContinue);

#endif
