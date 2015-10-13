//SERVER
#include "helpme.h"

struct client {
	int ID;
	char name[32];
	char pswd[32];
};

int main(int argc, char* argv[])
{
	int serverfd, port, err;
	struct sockaddr_in server_addr;
	struct client table[3];

	strncpy(table[0].name, "Adam", 4);
	strncpy(table[0].pswd, "password", 8);
	table[0].ID = 0;

	strncpy(table[1].name, "James", 5);
	strncpy(table[1].pswd, "password", 8);
	table[1].ID = 1;

	strncpy(table[2].name, "Forrest", 7);
	strncpy(table[2].pswd, "password", 8);
	table[2].ID = 2;


	if (argc != 2) {
		ErrorExit("ERROR wrong number of commands\n");
	}

	//will pick TCP if SOCK_STREAM is passed, which it is...
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0) {
		ErrorExit("ERROR opening socket\n");
	}

	//init struct to '0's
	memset(&server_addr, 0, sizeof(server_addr));
	port = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind the servers port to a socket
	err = bind(serverfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (err < 0) {
		ErrorExit("ERROR could not bind to socket\n");
	}

	err = listen(serverfd, 5);
	if (err < 0) {
		ErrorExit("Im a deaf mute server, har har har\n");
	}

	//like deadmau5 yo beep beep blurp blurp
	while (1<2) {

		int clientfd;
		int clientID;
		char* clientName;
		char* clientPswd;
		struct sockaddr clientAddr;
		unsigned int clientLen = sizeof(clientAddr);

		clientfd = accept(serverfd, (struct sockaddr*)&clientAddr, &clientLen);
		if (clientfd < 0) {
			ErrorExit("ERROR unable to accept client connection\n");
		}

		send_sentinel(clientfd, "Welcome", '\n');
		clientID = atoi(recv_sentinel(clientfd, SENTINEL));
		printf("ID: %d\n", clientID);

		clientName = recv_sentinel(clientfd, '\n');
		printf("Name: %s\n", clientName);

		if (clientID < sizeof(table) &&
			clientID >= 0 &&
			strcmp(table[clientID].name, clientName) == 0) {
			send_sentinel(clientfd, "Success", '\n');
		} else {
			send_sentinel(clientfd, "Failure", '\n');
			close(clientfd);
			continue;
		}

		clientPswd = recv_length(clientfd);
		if (strcmp(table[clientID].pswd, clientPswd) == 0) {
			char completeMsg[BUFFSIZE];
			sprintf(completeMsg, "Congratulations %s: you've just reveaked the password for %d to the world in plain-text!", clientName, clientID);
			send_length(clientfd, completeMsg);
		} else {
			send_length(clientfd, "Password incorrect");
		}
		close(clientfd);

	}
}

