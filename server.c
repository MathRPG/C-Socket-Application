#include <stdbool.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libnet.h>

#include "socket_utils.h"

void serve_connected_client(int sock_fd);

int main(void)
{
	const int server_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	socket_assert(server_fd, "Error creating the socket");

	socket_assert(
		setsockopt(
			server_fd, SOL_SOCKET, SO_REUSEADDR,
			&(int){ 1 }, sizeof(int)
		), "Error in setsockopt"
	);

	const struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(SERVER_PORT),
		.sin_addr.s_addr = INADDR_ANY,
		.sin_zero = { 0 },
	};

	socket_assert(
		bind(
			server_fd, CAST(&server_addr),
			sizeof(struct sockaddr)
		), "Socket Bind Failed"
	);

	socket_assert(
		listen(server_fd, SERVER_QUEUE_SIZE),
		"Error defining input queue size"
	);

	flush_printf("TCP Server waiting for clients at port %u\n", SERVER_PORT);

	const unsigned REQS_TO_PROCESS = 10;

	for (unsigned i = 0; i < REQS_TO_PROCESS; ++i)
	{
		struct sockaddr_in client_addr = {};

		const int client_fd = accept(
			server_fd,
			CAST(&client_addr),
			&(unsigned){ sizeof client_addr }
		);

		flush_printf(
			"Receiving connection (Client: %s, Port: %d)\n",
			inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port)
		);

		serve_connected_client(client_fd);
	}

	flush_printf("Processed %u requests already, shutting down...\n", REQS_TO_PROCESS);

	close(server_fd);

	return 0;
}

char* reverse_str(char* const s, const long len)
{
	char* l = s;
	char* r = s + len - 1;

	while (l < r)
	{
		char tmp = *l;
		*l = *r;
		*r = tmp;

		l++;
		r--;
	}

	return s;
}

void serve_connected_client(const int sock_fd)
{
	while (true)
	{
		struct RequestData data = { 0 };
		data.bytes_recv = recv(sock_fd, data.recv, MAX_MSG_LEN, NO_FLAGS);

		if (data.bytes_recv <= 0)
		{
			flush_printf("Lost connection\n");
			return;
		}

		flush_printf("Received message: {%s}\n", data.recv);

		bool connection_terminated = (
			strcmp(data.recv, "q") == 0 ||
			strcmp(data.recv, "Q") == 0
		);

		strcpy(data.send,
			connection_terminated
			? "Connection will be closed\n"
			: reverse_str(data.recv, data.bytes_recv));

		flush_printf("Sending message: {%s}\n", data.send);
		send(sock_fd, data.send, strlen(data.send), NO_FLAGS);

		if (connection_terminated)
		{
			flush_printf("Connection closed by client\n");
			close(sock_fd);
			return;
		}
	}
}
