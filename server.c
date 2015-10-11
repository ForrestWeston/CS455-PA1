//SERVER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 256

void ShowError(char* e)
{
	perror(e);
	exit(1);
}

int main(char* argv[], int arcg)
{
	//accept 2 command line arge, port to listen on
	//do not quite till Ctrl+C
	//once the client connects send 'Welcome\n'
	//recieve ID and Name, look up in table, send 'Success\n' or 'Failure\n'
	//if fail close socket
	//recv passwor verify with ID and Name,
	//don't know what this means, does the password have to be '<nameid>' possibly?
	//if correct respond 'Congratulations <name>; you've just revealed the password
	//for <ID number> to the world!'with 2-byte length
	//if fail respond 'Password Incorrect' with the 2-byte lenght

	int sockfd, cliSockfd, portNum, cliAddrLen, err;
	char buff[BUFFSIZE];
	struct sockaddr_in serv_addr, cli_addr;

	if (argc != 2) {
		ShowError("ERROR wrong number of commands\n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		ShowError("ERROR opening socket\n");
	}

	memset(&serv_addr, 0, sizeof(sockaddr));
	portNum = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portNum);
	serv_addr.sin_arrd.s_arrd = INADDR_ANY;

	err = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (err < 0) {
		ShowError("ERROR could not bind to socket\n");
	}

	listen(sockfd, 5);
	cliAddrLen = sizeof(cli_addr);
	cliSockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cliAddrLen);
	if (cliSockfd < 0) {
		ShowError("ERROR on accept");
	}


}
