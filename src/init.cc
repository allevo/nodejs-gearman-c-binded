
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif


#include "GearmanClient.h"
#include "GearmanTask.h"


void InitAll(Handle<Object> exports, Handle<Object> module) {
	GearmanClient::Init(exports);
	GearmanTask::Init(exports);
}

NODE_MODULE(gearmannodeCBinded, InitAll)
