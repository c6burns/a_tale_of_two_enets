#include "enet/enet.h"

#include <stdio.h>
#include <stdint.h>


int main(void)
{
	int ret;
	ENetAddress address;
	ENetHost *host;
	ENetEvent event;


	if (enet_initialize() != 0) {
		printf("An error occurred while initializing ENet\n");
		goto cleanup;
	}
	atexit(enet_deinitialize);

	// addresses are all forced ipv6 in enet_csharp, with no interface to set scope
	// so just zero everything
	memset(&address, 0, sizeof(address));
	address.host = ENET_HOST_ANY;
	address.port = 65432;
	if (!(host = enet_host_create(&address, 32, 2, 0, 0))) {
		printf("An error occurred while trying to create an ENet host\n");
		goto cleanup;
	}

	while ((ret = enet_host_service(host, &event, 1000)) >= 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_CONNECT:
			printf("A new client connected\n");
			event.peer->data = "test client";
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf("Recv on channel %u -- %zu bytes\n", event.channelID, (uint64_t)event.packet->dataLength);
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected\n", (char *)event.peer->data);
			event.peer->data = NULL;
			break;
		}
	}

	printf("enet_host_service exited with: %d\n", ret);

cleanup:
	enet_host_destroy(host);

	return 0;
}
