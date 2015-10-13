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
	strncpy(buff, msg, msgSize);

	err = send(socket, buff, msgSize, 0);
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

char* recv_length(int socket)
{
	char buff[BUFFSIZE];
	char* msg;
	char* msgRaw = buff + sizeof(uint16_t);
	uint16_t size;
	int err;

	err = recv(socket, buff, BUFFSIZE, 0);
	if (err < 0) {
		return NULL;
	}
	size = *((uint16_t*)buff);
	size = ntohs(size);
	msg = malloc(size);
	if (msg < 0) {
		return NULL;
	}
	memcpy(msg, msgRaw, size);
	msg[size-1] = '\0';
	return msg;
}

