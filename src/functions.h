#ifndef __GEARMANFUNCTIONS_H__
#define __GEARMANFUNCTIONS_H__


#include <node.h>
#include <nan.h>
using namespace v8;
using namespace node;

#include <libgearman/gearman.h>

NAN_METHOD(strerror);
NAN_METHOD(gearman_success);
NAN_METHOD(gearman_failed);
NAN_METHOD(gearman_continue);

#endif
