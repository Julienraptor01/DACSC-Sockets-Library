#include "test.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socketTCP.h"

int main(int argc, char *argv[]) {
	argument_t argument = ARGUMENT_DEFAULT;
	if (argc != 3) {
		argument = ARGUMENT_INVALID;
	} else if (strcmp(argv[1], "server") == 0) {
		argument = ARGUMENT_SERVER;
	} else if (strcmp(argv[1], "client") == 0) {
		argument = ARGUMENT_CLIENT;
	} else {
		argument = ARGUMENT_INVALID;
	}
	switch (argument) {
		case ARGUMENT_SERVER: {
			ipv4Address interfaceIP = {.address = 0};
			if (!stringToIPv4Address(argv[2], &interfaceIP)) {
				printf("Failed to parse interface IP address\n");
				return 1;
			}
			char interfaceIPString[16];
			ipv4AddressToString(interfaceIP, interfaceIPString);
			printf("Using interface %s\n", interfaceIPString);
			int serverSocket = createServerSocket(6942, &interfaceIP);
			if (serverSocket < 0) {
				printf("Failed to create server socket\n");
				return 1;
			}
			ipv4Address clientIP;
			int clientSocket = acceptClient(serverSocket, &clientIP);
			if (clientSocket < 0) {
				printf("Failed to accept client connection\n");
				return 1;
			}
			char clientIPString[16];
			ipv4AddressToString(clientIP, clientIPString);
			printf("Accepted connection from %s\n", clientIPString);
			uint8_t buffer[4];
			if (!receive(clientSocket, buffer, 4)) {
				printf("Failed to receive data\n");
				return 1;
			}
			printf("Received %zu bytes:", sizeof(buffer));
			for (size_t i = 0; i < sizeof(buffer); i++) {
				printf(" %02x", buffer[i]);
			}
			printf("\n");
			uint8_t data[] = {0xca, 0xfe, 0xba, 0xbe};
			if (!send(clientSocket, data, sizeof(data))) {
				printf("Failed to send data\n");
				return 1;
			}
			printf("Sent %zu bytes:", sizeof(data));
			for (size_t i = 0; i < sizeof(data); i++) {
				printf(" %02x", data[i]);
			}
			printf("\n");
			break;
		}
		case ARGUMENT_CLIENT: {
			ipv4Address serverIP = {.address = 0};
			if (!hostnameToIPv4Address(argv[2], &serverIP)) {
				printf("Failed to resolve hostname\n");
				return 1;
			}
			char serverIPString[16];
			ipv4AddressToString(serverIP, serverIPString);
			printf("Resolved \"%s\" to %s\n", argv[2], serverIPString);
			int clientSocket = createClientSocket(serverIP, 6942);
			if (clientSocket < 0) {
				printf("Failed to create client socket\n");
				return 1;
			}
			uint8_t data[] = {0xde, 0xad, 0xbe, 0xef};
			if (!send(clientSocket, data, sizeof(data))) {
				printf("Failed to send data\n");
				return 1;
			}
			printf("Sent %zu bytes:", sizeof(data));
			for (size_t i = 0; i < sizeof(data); i++) {
				printf(" %02x", data[i]);
			}
			printf("\n");
			uint8_t buffer[4];
			if (!receive(clientSocket, buffer, 4)) {
				printf("Failed to receive data\n");
				return 1;
			}
			printf("Received %zu bytes:", sizeof(buffer));
			for (size_t i = 0; i < sizeof(buffer); i++) {
				printf(" %02x", buffer[i]);
			}
			printf("\n");
			break;
		}
		case ARGUMENT_INVALID:
		case ARGUMENT_DEFAULT:
		default:
			printf("Usage: %s <client/server> <hostname/interface>\n", argv[0]);
			return 42069;
	}
}
