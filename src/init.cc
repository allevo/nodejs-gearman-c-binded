
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif


#include "GearmanClient.h"


void InitAll(Handle<Object> exports, Handle<Object> module) {
	GearmanClient::Init(exports);
}

NODE_MODULE(gearmannodeCBinded, InitAll)
