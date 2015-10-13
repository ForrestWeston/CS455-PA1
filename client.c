//CLIENT
#include "helpme.h"

int main(int argc, char* argv[])
{
	struct sockaddr_in server_addr;
	struct hostent *server;
	int sockfd, err;
	char Pswd[32];
	char Name[36];
	char ID[16];
	char* correct;
	char* welcome;
	const char sentinel = '\n';

	if (argc != 3) {
		ErrorExit("ERROR wrong number of commands\n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		ErrorExit("ERROR opening socket\n");
	}

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		printf("ERROR no such host\n");
		exit(0);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy(&server_addr.sin_addr, server->h_addr_list[0], server->h_length);
	server_addr.sin_port = htons(atoi(argv[2]));

	err = connect(sockfd, (struct  sockaddr *) &server_addr, sizeof(server_addr));
	if (err < 0) {
		ErrorExit("ERROR on connect");
	}

	welcome = recv_sentinel(sockfd, sentinel);
	puts(welcome);
	if (strcmp(welcome, "Welcome") !=0 ) {
		ErrorExit("ERROR message recieved was not 'Welcome'\n");
	}

	printf("Enter ID: ");
	scanf("%s", ID);
	send_sentinel(sockfd, ID, sentinel);

	printf("Enter Name: ");
	scanf("%s", Name);
	send_sentinel(sockfd, Name, sentinel);

	correct = recv_sentinel(sockfd, sentinel);
	printf("Name: %s, ID: %s, Correct?: %s\n", Name, ID, correct);
	if (strcmp(correct, "Success") != 0) {
		ErrorExit("ERROR Incorrect Name or ID\n");
	}

	printf("Enter Password: ");
	scanf("%s", Pswd);
	send_length(sockfd, Pswd);

	correct = recv_length(sockfd);
	printf("%s\n", correct);

	close(sockfd);
	return 0;


}
