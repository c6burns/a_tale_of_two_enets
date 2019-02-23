#include "enet/enet.h"

#include <stdio.h>


int main(void)
{
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	return 0;
}
