#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libnet.h>

#include "socket_utils.h"

void make_requests_to_server(int sockfd);

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [SERVER_ADDRESS]", argv[0]);
		exit(1);
	}

	const struct hostent* host = gethostbyname(argv[1]);

	int sockfd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	socket_assert(sockfd, "Error creating the socket");

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_PORT),
		.sin_addr = *(struct in_addr*)host->h_addr,
		.sin_zero = { 0 },
	};

	socket_assert(
		connect(
			sockfd,
			CAST(&server_addr),
			sizeof(struct sockaddr)
		), "Connection error"
	);

	make_requests_to_server(sockfd);

	return 0;
}

void remove_trailing_newline(char* const str)
{
	size_t index = strcspn(str, "\n");
	str[index] = '\0';
}

void make_requests_to_server(const int sockfd)
{
	while (true)
	{
		struct RequestData data = { 0 };

		flush_printf("Input text ('Q' or 'q' to quit) : ");

		fgets(data.send, sizeof data.send, stdin);
		remove_trailing_newline(data.send);

		flush_printf("Sending message: {%s}\n", data.send);
		send(sockfd, data.send, strlen(data.send), NO_FLAGS);

		data.bytes_recv = recv(sockfd, data.recv, MAX_MSG_LEN, NO_FLAGS);
		data.recv[data.bytes_recv] = '\0';

		bool connection_will_close = (
			strcmp(data.recv, "Connection will be closed\n") == 0
		);

		flush_printf("Received message: {%s}\n", data.recv);

		if (connection_will_close)
		{
			flush_printf("Closing connection to server\n");
			close(sockfd);
			return;
		}
	}
}
