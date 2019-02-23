#include "enet/enet.h"

#include <stdio.h>
#include <stdint.h>


int main(void)
{
	int ret;
	const char *packet_msg = "I love chocolate, but I can't eat it or else I'll get FAT!";
	size_t packet_msg_len = strlen(packet_msg) + 1;
	ENetAddress address;
	ENetHost *host = NULL;
	ENetPeer *peer = NULL;
	ENetEvent event;


	if (enet_initialize() != 0) {
		printf("An error occurred while initializing ENet\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);


	if (!(host = enet_host_create(NULL, 1, 2, 0, 0))) {
		printf("An error occurred while trying to create an ENet host\n");
		goto cleanup;
	}

	// addresses are all ipv6 in enet csharp so update this
	memset(&address, 0, sizeof(address));
	if ((ret = enet_address_set_host(&address, "192.168.86.233"))) {
		printf("enet_address_set_host failed\n");
		goto cleanup;
	}
	address.port = 65432;

	if (!(peer = enet_host_connect(host, &address, 2, 0))) {
		fprintf(stderr, "No available peers for initiating an ENet connection\n");
		goto cleanup;
	}
	peer->data = "my speshul connexion";

	if ((ret = enet_host_service(host, &event, 5000)) <= 0) {
		printf("Connection failed: %d", ret);
		goto cleanup;
	}

	if (event.type != ENET_EVENT_TYPE_CONNECT) {
		printf("Connection failed -- excpected ENET_EVENT_TYPE_CONNECT but recvd: %d\n", event.type);
		goto cleanup;
	}


	while (1) {
		ENetPacket *packet;
		if (!(packet = enet_packet_create(packet_msg, packet_msg_len, ENET_PACKET_FLAG_RELIABLE))) {
			printf("enet_packet_create failed (probably ENOMEM)\n");
			goto cleanup;
		}

		if ((ret = enet_peer_send(peer, 0, packet))) {
			printf("enet_peer_send error: %d", ret);
			goto cleanup;
		}

		enet_host_flush(host);

		// process all outstanding recv
		while (enet_host_service(host, &event, 0) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				printf("Recv channel %u -- %zu bytes\n", event.channelID, (uint64_t)event.packet->dataLength);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", (char *)event.peer->data);
				event.peer->data = NULL;
				goto cleanup;
			default:
				printf("Did not expect event.type: %d\n", event.type);
				goto cleanup;
			}
		}
	}

cleanup:
	if (peer) enet_peer_reset(peer);
	if (host) enet_host_destroy(host);

	return 0;
}
