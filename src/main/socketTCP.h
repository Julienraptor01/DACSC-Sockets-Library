#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include <sys/types.h>

#include "networkTools.h"

/**
 * @brief Create a server socket
 * @param interfaceIP The IP address of the interface to bind to
 * @param serverPort The port to listen on
 * @return The server socket
 */
int createServerSocket(ipv4Address interfaceIP, unsigned short serverPort);
/**
 * @brief Accept a client connection
 * @param serverSocket The server socket
 * @param clientIP The IP address of the client
 * @return The client socket
 * @note The clientIP parameter is an output parameter
 */
int acceptClient(int serverSocket, ipv4Address *clientIP);
/**
 * @brief Create a client socket
 * @param serverIP The IP address of the server
 * @param serverPort The port of the server
 * @return The client socket
 */
int createClientSocket(ipv4Address serverIP, unsigned short serverPort);

/**
 * @brief Send data over a socket
 * @param socket The socket to send data over
 * @param data The data to send
 * @param size The size of the data to send
 * @return If the data was sent successfully
 */
bool send(int socket, const uint8_t data[], ssize_t size);
/**
 * @brief Receive data from a socket
 * @param socket The socket to receive data from
 * @param data The buffer to store the received data
 * @param size The size of the buffer
 * @return If the data was received successfully
 */
bool receive(int socket, uint8_t data[], ssize_t size);

#endif
