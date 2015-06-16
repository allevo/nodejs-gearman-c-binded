
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif


#include "GearmanClient.h"
#include "MultipleTask.h"


void InitAll(Handle<Object> exports, Handle<Object> module) {
	GearmanClient::Init(exports);
	MultipleTask::Init(exports);
}

NODE_MODULE(gearmannodeCBinded, InitAll)
