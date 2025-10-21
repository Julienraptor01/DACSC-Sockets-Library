#ifndef TEST_H
#define TEST_H

#include <cstddef>
#include <cstdint>

enum argument_t {
	ARGUMENT_DEFAULT,
	ARGUMENT_INVALID,
	ARGUMENT_SERVER,
	ARGUMENT_CLIENT
};

int main(int argc, char *argv[]);
argument_t parseArguments(int argc, char *argv[]);
bool server(char *interface);
bool client(char *hostname);
void printHexBuffer(uint8_t buffer[], size_t length);

#endif
