#include "test.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socketTCP.h"

int main(int argc, char *argv[]) {
	switch (parseArguments(argc, argv)) {
		case ARGUMENT_SERVER:
			return server(argv[2]) ? 0 : 1;
		case ARGUMENT_CLIENT:
			return client(argv[2]) ? 0 : 1;
		case ARGUMENT_INVALID:
		case ARGUMENT_DEFAULT:
		default:
			printf("Usage: %s <client/server> <hostname/interface>\n", argv[0]);
			return 42069;
	}
}

argument_t parseArguments(int argc, char *argv[]) {
	if (argc != 3) {
		return ARGUMENT_INVALID;
	} else if (strcmp(argv[1], "server") == 0) {
		return ARGUMENT_SERVER;
	} else if (strcmp(argv[1], "client") == 0) {
		return ARGUMENT_CLIENT;
	} else {
		return ARGUMENT_INVALID;
	}
}

bool server(char *interface) {
	ipv4Address interfaceIP = {.address = 0};
	if (!stringToIPv4Address(interface, &interfaceIP)) {
		printf("Failed to parse interface IP address\n");
		return false;
	}
	char interfaceIPString[16];
	ipv4AddressToString(interfaceIP, interfaceIPString);
	printf("Using interface %s\n", interfaceIPString);
	int serverSocket = createServerSocket(6942, &interfaceIP);
	if (serverSocket < 0) {
		printf("Failed to create server socket\n");
		return false;
	}
	ipv4Address clientIP;
	int clientSocket = acceptClient(serverSocket, &clientIP);
	if (clientSocket < 0) {
		printf("Failed to accept client connection\n");
		return false;
	}
	char clientIPString[16];
	ipv4AddressToString(clientIP, clientIPString);
	printf("Accepted connection from %s\n", clientIPString);
	uint8_t buffer[4];
	if (!receive(clientSocket, buffer, 4)) {
		printf("Failed to receive data\n");
		return false;
	}
	printf("Received %zu bytes:", sizeof(buffer));
	printHexBuffer(buffer, sizeof(buffer));
	uint8_t data[] = {0xca, 0xfe, 0xba, 0xbe};
	if (!send(clientSocket, data, sizeof(data))) {
		printf("Failed to send data\n");
		return false;
	}
	printf("Sent %zu bytes:", sizeof(data));
	printHexBuffer(data, sizeof(data));
	return true;
}

bool client(char *hostname) {
	ipv4Address serverIP = {.address = 0};
	if (!hostnameToIPv4Address(hostname, &serverIP)) {
		printf("Failed to resolve hostname\n");
		return false;
	}
	char serverIPString[16];
	ipv4AddressToString(serverIP, serverIPString);
	printf("Resolved \"%s\" to %s\n", hostname, serverIPString);
	int clientSocket = createClientSocket(serverIP, 6942);
	if (clientSocket < 0) {
		printf("Failed to create client socket\n");
		return false;
	}
	uint8_t data[] = {0xde, 0xad, 0xbe, 0xef};
	if (!send(clientSocket, data, sizeof(data))) {
		printf("Failed to send data\n");
		return false;
	}
	printf("Sent %zu bytes:", sizeof(data));
	printHexBuffer(data, sizeof(data));
	uint8_t buffer[4];
	if (!receive(clientSocket, buffer, 4)) {
		printf("Failed to receive data\n");
		return false;
	}
	printf("Received %zu bytes:", sizeof(buffer));
	printHexBuffer(buffer, sizeof(buffer));
	return true;
}

void printHexBuffer(uint8_t buffer[], size_t length) {
	for (size_t i = 0; i < length; i++) {
		printf(" %02x", buffer[i]);
	}
	printf("\n");
}
