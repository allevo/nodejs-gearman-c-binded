#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgearman/gearman.h>

void* myfunc(gearman_job_st *job, void *data, size_t *size, gearman_return_t *ret) {

    *ret=GEARMAN_SUCCESS;
    char *result=(char*)calloc(30,sizeof(char));
    strcpy(result,"work completed");
    printf("%s|%s|%s|%s\n", (char*) gearman_job_workload(job), gearman_job_unique(job), gearman_job_handle(job), gearman_job_function_name(job));
    *size=30;
    return result;
}

int main(int argc, char** argv) {
	gearman_worker_st worker;
    gearman_worker_create(&worker);
    gearman_worker_add_server(&worker, "127.0.0.1", 4731);
    gearman_worker_set_timeout(&worker, 1000);
	gearman_return_t ret = gearman_worker_add_function(&worker, argv[1], 0, myfunc, NULL);

	while(!gearman_worker_work(&worker)) { }

	gearman_worker_free(&worker);
	return 0;
}
