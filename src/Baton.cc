
#include "Baton.h"

void JobBackgroundBaton::Execute(WrapGearmanClient* client) {
	task->handle = new char[GEARMAN_JOB_HANDLE_SIZE];
	task->ret = gearman_client_do_background(client->client, task->queue, task->unique, task->data, strlen(task->data), task->handle);
	client->debug && printf("%s %s %s %s %d %s\n", "AAAA", task->queue, task->unique, task->data, (int) strlen(task->data), task->handle);
}
