#include "helpme.h"

void ErrorExit(char* e)
{
	perror(e);
	exit(1);
}

int send_sentinel(int socket, char* msg, const char sentinel)
{
	char buff[BUFFSIZE];
	int err;
	int msgSize = strlen(msg);
	strncpy(buff, msg, msgSize);

	buff[msgSize++] = sentinel;
	err = send(socket, buff, msgSize, 0);
	if (err < 0) {
		return -1;
	}
	return msgSize;
}

int send_length(int socket, char* msg)
{
	char buff[BUFFSIZE];
	int err;
	int msgSize = strlen(msg);
	memcpy(&buff[2], msg, msgSize);

	uint16_t size = msgSize;
	memcpy(buff, &size, sizeof(uint16_t));
	buff[2+msgSize] = 0;

	// send 2 byte length, entire message, and null byte
	err = send(socket, buff, msgSize + 3, 0);
	if (err < 0) {
		return -1;
	}
	return msgSize;
}

char* recv_sentinel(int socket, const char sentinel)
{
	char buff[BUFFSIZE];
	int size;
	char* msg;

	size = recv(socket, buff, BUFFSIZE, 0);
	if (size < 0) {
		return NULL;
	}

	msg = malloc(size);
	if (msg < 0) {
		return NULL;
	}

	memcpy(msg, buff, size);
	msg[size-1] = '\0';
	return msg;

}

int recv_bytes(int fd, char *buf, int len) {
	int recvd = 0;
	while (recvd < len) {
		int n = recv(fd, &buf[recvd], len - recvd, 0);
		if (n < 0) {
			return n;
		}

		recvd += n;
	}
	return recvd;
}

char* recv_length(int socket)
{
	char buff[BUFFSIZE];
	char* msg;
	uint16_t size;
	int err;

	// recieve length
	err = recv_bytes(socket, buff, 2);
	if (err < 0) {
		return NULL;
	}

	size = *((uint16_t*)buff);
	printf("size is %d\n", size);

	msg = malloc(size + 1);
	if (msg < 0) {
		return NULL;
	}

	err = recv_bytes(socket, msg, size);
	if (err < 0) {
		return NULL;
	}

	return msg;
}

