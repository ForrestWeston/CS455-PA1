// Adam Rodriguez, James Jessen, Forrest Weston
// Team: TCP
// Computer Networks - Project 1
#include "utilfunctions.c"

#define BUFSIZE 512

int hostToIp(char *hostname, char *ip);

int main(int argc, char *argv[])
{
    int sockFd, portNumber, msgLen = 0, totalLen = 0;
    struct sockaddr_in serv_addr;
    char ip[64], message[BUFSIZE], buf[BUFSIZE], buf2[BUFSIZE];
    
    // Verify num of arguments
    if (argc < 3)
    {
        printf("missing arguments. usage: %s serverName portNumber\n", argv[0]);
        return -1;
    }
    
    // get portNum and IP from command line
    portNumber = atoi(argv[2]);
    
    if (hostToIp(argv[1], ip) < 0)
    {
        printf("failed to get IP address. Exiting...\n");
        return -1;
    }
    
    // Create socket for client
    if ((sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("socket() failed. Exiting...");
        return -1;
    }
    
    // Setup the server address with IP and portNum
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(portNumber);
    
    // Attempt to connect to server
    if ((connect(sockFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("connect() failed. Exiting...");
        return -1;
    }
    
    printf("connection established...\n");
    
    // read Welcome message from server
    readNl(sockFd, message);
    printf("Client Received: %s", message);
    
    // Verify the welcome message
    if ( memcmp(message, "Welcome\n", nlStrLen(message)) != 0 )
    {
        printf("Welcome message received was incorrect from expected... exiting...\n");
        close(sockFd);
        return -1;
    }
    
    // Welome Verification Successful
    
    printf("Enter ID Number & Name [idNum name]: ");
    scanf("%s %s", buf, buf2);
    
    msgLen = strlen(buf);
    buf[msgLen] = '\n'; // replace '\0' with '\n'
    
    msgLen = strlen(buf2);
    buf2[msgLen] = '\n'; // replace '\0' with '\n'
    
    // Send id number to server
    msgLen = nlStrLen(buf) + 1;
    if ((send(sockFd, buf, msgLen, 0)) != msgLen)
    {
        printf("send() failed... exiting..\n");
        close(sockFd);
        return -1;
    }
    
    // Send name to server
    msgLen = nlStrLen(buf2) + 1;
    if ((send(sockFd, buf2, msgLen, 0)) != msgLen)
    {
        printf("send() failed... exiting..\n");
        close(sockFd);
        return -1;
    }
    
    // read Success/Failure message from server
    readNl(sockFd, message);
    printf("Client Received: %s", message);

    if (memcmp(message, "Failure", nlStrLen(message)) == 0)
    {
        // Invalid ID & Name given
        printf("ID and Name not found. Closing connection...");
        close(sockFd);
        return -1;
    }

    printf("Enter password: "); // get password from client
    scanf("%s", buf);

    // send password
    msgLen = strlen(buf);
    writeLen(sockFd, buf, msgLen);

    //Read in message
    readLen(sockFd, message, 2);
    printf("Client received: %s\n", message);
    
    close(sockFd);
    return 0;
}

// convert a name to IP using gethostbyname and place in *ip
int hostToIp(char *hostname, char *ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    
    if ( (he = gethostbyname(hostname)) == NULL )
    {
        return -1;
    }
    
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for (i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 0;
    }
    
    return -1;
}