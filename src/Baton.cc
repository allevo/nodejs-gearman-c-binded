
#include "Baton.h"

void JobBackgroundBaton::Execute(WrapGearmanClient* client) {
	task->handle = new char[GEARMAN_JOB_HANDLE_SIZE];
	client->debug && printf("%s\n", "doing do_backgorund");
	task->ret = gearman_client_do_background(client->client, task->queue, task->unique, task->data, strlen(task->data), task->handle);
	client->debug && printf("%s %s %s %s %d %s %s\n", "do_backgorund done", task->queue, task->unique, task->data, (int) strlen(task->data), task->handle, gearman_strerror(task->ret));
	this->setResult(gearman_success(task->ret));
}


void JobStatusBaton::Execute(WrapGearmanClient* client) {
	ret = gearman_client_job_status(client->client, handle, &is_known, &is_running, &numerator, &denominator);
	client->debug && printf("%s %s %d %d %d %d\n", "job_status done", handle, is_known, is_running, numerator, denominator);
	this->setResult(gearman_success(ret));
}
