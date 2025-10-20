#include "socketTCP.h"

#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int createServerSocket(unsigned short listeningPort, ipv4Address *interfaceIP) {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		return -1;
	}
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(listeningPort);
	serverAddress.sin_addr.s_addr = interfaceIP->address;
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(sockaddr_in)) < 0) {
		close(serverSocket);
		return -1;
	}
	if (listen(serverSocket, 1) < 0) {
		close(serverSocket);
		return -1;
	}
	return serverSocket;
}

int acceptClient(int serverSocket, ipv4Address *clientIP) {
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(sockaddr_in);
	int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientSocket < 0) {
		return -1;
	}
	clientIP->address = clientAddress.sin_addr.s_addr;
	return clientSocket;
}

int createClientSocket(ipv4Address serverIP, unsigned short serverPort) {
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		return -1;
	}
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort);
	serverAddress.sin_addr.s_addr = serverIP.address;
	if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(sockaddr_in)) < 0) {
		close(clientSocket);
		return -1;
	}
	return clientSocket;
}

bool send(int socket, const uint8_t data[], ssize_t size) {
	for (ssize_t totalSent = 0, sent = 0; totalSent < size; totalSent += sent) {
		sent = write(socket, data + totalSent, size - totalSent);
		if (sent <= 0) {
			return false;
		}
	}
	return true;
}

bool receive(int socket, uint8_t data[], ssize_t size) {
	for (ssize_t totalReceived = 0, received = 0; totalReceived < size; totalReceived += received) {
		received = read(socket, data + totalReceived, size - totalReceived);
		if (received <= 0) {
			return false;
		}
	}
	return true;
}
