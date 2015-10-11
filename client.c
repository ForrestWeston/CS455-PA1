//CLIENT
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 256

void ShowError(char *e)
{
	perror(e);
	printf("USAGE: run <IP Addr> <Port>\n");
	exit(1);
}

int main(char* argv[], int arcg)
{
	//accept 2 command line args, IP address of server, port server is listening
	//connect upon recieveing cmd inputs
	//if 'Welcome\n is not recieved close connection, print error message, exit
	//else promt user for ID and Name lol hausers code is wrong for this part
	//if !success, show failure, exit. !do not sent to closed socket!
	//prompt user for password, send 1.length 2.password
	//1.lenght: 2-byte binary number in network byte order htons()
	//2.string: non-null terminatedo
	//appon recv result of password, print, close socket, exit

	struct sockaddr_in server_addr;
	struct hostent *server;
	int sockfd, portnum, err;
	char buff[BUFFSIZE];

	if (argc != 3) {
		ShowError("ERROR wrong number of commands\n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		ShowError("ERROR opening socket\n");
	}

	portnum = atoi(argv[2]);
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		printf("ERROR no such host\n");
		exit(0);
	}

	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	memcpy(server->h_addr, &server_addr.sin_addr.s_arrd, server->h_length);
	server_addr.sin_port = htons(portnum);

	// Attempt to connect to the server
	err = connect(sockfd, &server, sizeof(server_addr));
	if (err < 0) {
		ShowError("ERROR on connect\n");
	}
}
