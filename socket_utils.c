#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "socket_utils.h"

const int SERVER_PORT = 5000;
const int SERVER_QUEUE_SIZE = 2;

#define SOCKET_ERROR_VAL (-1)

void socket_assert(int ret, const char* fmt, ...)
{
	if (ret != SOCKET_ERROR_VAL)
		return;

	va_list args;
	va_start(args, fmt);

#define SOCKET_ASSERT_BUFF_SIZE 512
	char buff[SOCKET_ASSERT_BUFF_SIZE];
	vsnprintf(buff, SOCKET_ASSERT_BUFF_SIZE, fmt, args);

	perror(buff);
	exit(1);
}

void flush_printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	vprintf(fmt, args);
	fflush(stdout);

	va_end(args);
}
