#ifndef _SOCKET_UTILS_H_
#define _SOCKET_UTILS_H_

void socket_assert(int ret, const char* fmt, ...);
void flush_printf(const char* fmt, ...);

#define CAST(addr) (struct sockaddr*)addr

#define NO_FLAGS 0

#define DEFAULT_PROTOCOL 0

extern const int SERVER_PORT;
extern const int SERVER_QUEUE_SIZE;

#define MAX_MSG_LEN 512

struct RequestData
{
	char recv[MAX_MSG_LEN];
	long bytes_recv;
	char send[MAX_MSG_LEN];
};

#endif //_SOCKET_UTILS_H_
