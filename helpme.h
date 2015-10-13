#ifndef __HELPME_H__
#define __HELPME_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define BUFFSIZE 256

int send_sentinel(int socket, char* msg, const char sentinel);
int send_length(int socket, char* msg);

char* recv_sentinel(int socket, const char sentinel);
char* recv_length(int socket);

void ErrorExit(char* e);

#endif
