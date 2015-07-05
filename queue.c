#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgearman/gearman.h>

int main(int argc, char** argv) {
	gearman_client_st *client= gearman_client_create(NULL);

	gearman_client_set_timeout(client, 1800);
	gearman_return_t ret= gearman_client_add_server(client, "127.0.0.1", 4731);
	if (gearman_failed(ret)) {
		return EXIT_FAILURE;
	}

	size_t result_size;
	gearman_return_t rc;
	printf("AAAAAA\n");
	void* value= gearman_client_do(client, argv[1], argv[2], argv[3], strlen(argv[3]), &result_size, &rc);
	printf("AAAAAA\n");

	if (gearman_success(rc)) {
		printf("DONE|%s\n", (char*) value);
	}
	free(value);
	gearman_client_free(client);

	return 0;
}
