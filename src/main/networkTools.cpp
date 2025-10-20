#include "networkTools.h"

#include <cstdio>
#include <netdb.h>

bool stringToIPv4Address(const char string[], ipv4Address *address) {
	uint8_t a, b, c, d;
	if (sscanf(string, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d) != 4) {
		return false;
	}
	address->bytes[0] = a;
	address->bytes[1] = b;
	address->bytes[2] = c;
	address->bytes[3] = d;
	return true;
}

void ipv4AddressToString(ipv4Address address, char string[]) {
	sprintf(string, "%hhu.%hhu.%hhu.%hhu", address.bytes[0], address.bytes[1], address.bytes[2], address.bytes[3]);
}

bool hostnameToIPv4Address(const char hostname[], ipv4Address *address) {
	struct addrinfo hints = {
		.ai_flags = 0,
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM,
		.ai_protocol = 0,
		.ai_addrlen = 0,
		.ai_addr = nullptr,
		.ai_canonname = nullptr,
		.ai_next = nullptr,
	};
	struct addrinfo *results;
	int status = getaddrinfo(hostname, nullptr, &hints, &results);
	if (status != 0) {
		return false;
	}
	address->address = ((struct sockaddr_in *)results->ai_addr)->sin_addr.s_addr;
	freeaddrinfo(results);
	return true;
}
