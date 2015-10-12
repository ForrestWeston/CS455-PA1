// Adam Rodriguez, James Jessen, Forrest Weston
// Team: TCP
// Computer Networks - Project 1
#include "utilfunctions.c"

#define BUFSIZE 512

int isPersonValid(char *id, char *name);
int isPasswordValid(char *pw, int pw_len, int p_index);

char *id_table[5] = {"11334", "12345", "54321", "24135", "13524"};
char *name_table[5] = {"adam", "doug", "forrest", "bob", "james"};
char *pw_table[5] = {"pw", "password", "admin", "host", "bond"};

int main(int argc, char *argv[])
{
    int sockFd, connFd, portNum, clientLen, msgLen, totalLen, table_index, len;
    struct sockaddr_in serv_addr; // Server address
    struct sockaddr_in client_addr; // Client address
    char message[BUFSIZE], buf[BUFSIZE], id[64], name[64];

    if (argc < 2)
    {
        printf("missing arguments. usage: %s portNumber\n", argv[0]); // incorrect amount of args given
        return -1;
    }

    portNum = atoi(argv[1]); // get portnum

    // create socket for server
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket() failed. exiting...\n");
        return -1;
    }

    // set up server address and port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portNum);

    // bind server address to socket
    if ((bind(sockFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr) )) == -1)
    {
        printf("bind() failed. exiting...\n");
        return -1;
    }

    // set socket to listen to max 1 connection
    if ((listen(sockFd, 1)) == -1)
    {
        printf("listen() failed. exiting...\n");
        return -1;
    }

    printf("Connection established on port %d\n", portNum);
    
    // Server loop
    while (1)
    {
        // Wait for connection to be accepted from client
        clientLen = sizeof(client_addr);
        if ((connFd = accept(sockFd, (struct sockaddr*)&client_addr, &clientLen)) < 0)
        {
            printf("accept() failed. exiting...\n");
            return -1;
        }

        // Prepare to send welcome message
        strcpy(message, "Welcome");
        msgLen = strlen(message);
        message[msgLen] = '\n'; // replace '\0' with '\n'
        msgLen ++; // Add 1 to have message length include '\n'
        
        // Send welcome message to client
        if ((send(connFd, message, msgLen, 0)) != msgLen)
        {
            printf("write() failed... exiting..\n");
            close(connFd);
            return -1;
        }
        
        printf("Server now waiting on data from client...\n");
        
        // read ID number from client
        readNl(connFd, id);

        // Read name from client
        readNl(connFd, name);
        
        printf("ID: %s NAME: %s", id, name);
        
        table_index = isPersonValid(id, name); // get index of person; -1 if not valid
        
        if (table_index == -1) // Invalid ID & Name
        {
            // Prepare to send Failure message
            strcpy(message, "Failure");
            msgLen = strlen(message);
            message[msgLen] = '\n'; // replace '\0' with '\n'
            msgLen ++; // Add 1 to have message length include '\n'
            
            // Send Failure message to client
            if ((send(connFd, message, msgLen, 0)) != msgLen)
            {
                printf("write() failed... exiting..\n");
                return -1;
            }
            
            // Close client socket
            close(connFd);
            printf("invalid ID and NAME.. Client connection closed... now accepting connections...\n");
            continue;
        }

        // Prepare to send Success message
        strcpy(message, "Success");
        msgLen = strlen(message);
        message[msgLen] = '\n'; // replace '\0' with '\n'
        msgLen ++; // Add 1 to have message length include '\n'
        
        // Send Success message to client
        if ((send(connFd, message, msgLen, 0)) != msgLen)
        {
            printf("write() failed... exiting..\n");
            return -1;
        }

        // Read password from client
        msgLen = readLen(connFd, message, 1);

        // Validate password
        if (isPasswordValid(message, msgLen, table_index))
        {
            // prepare congrats message
            strcpy(buf, "Congratulations ");
            strncat(buf, name, nlStrLen(name));
            strcat(buf, "; you've just revealed the password for ");
            strncat(buf, id, nlStrLen(id));
            strcat(buf, " to the world!");
        }
        else
        {
            // prepare invalid message
            strcpy(buf, "Password incorrect.");
        }

        // Send congrats/invalid message to client
        len = strlen(buf);
        if (writeLen(connFd, buf, len) < 0)
        {
            printf("sending \"%s\" failed. Exiting...\n", buf);
            close(connFd);
            return -1;
        }
  
        close(connFd);
        printf("Client connection closed... now accepting connections...\n");
    }
    
    close(sockFd);
}

// check if id and name are in the table
// returns index of person if id and name are in table; return -1 otherwise
int isPersonValid(char *id, char *name)
{
    int i = 0, result = 0, len = 0;
    int person_index = -1;
    
    if (*id == 0 || *name == 0)
    {
        return -1; // id or name is empty
    }
    
    // Search id table for any matches on given id
    len = nlStrLen(id);
    for (i = 0; i < 5; i++)
    {
        result = memcmp(id, id_table[i], len);
        
        if (result == 0)
        {
            person_index = i; // get index if given id matches one in the table
            break;
        }
    }
    
    if (person_index == -1)
    {
        return -1; // given id doesn't match any in id table
    }
    
    len = nlStrLen(name);
    result = memcmp(name, name_table[person_index], len); // Compare given name with name table
    if (result == 0)
    {
        return person_index; // person found in id/name table
    }
    
    return -1; // name does not match given id
}

// check if password is correct for given id and name
// returns 1 if password is valid; returns 0 otherwise
int isPasswordValid(char *pw, int pw_len, int p_index)
{
    int result = 0, len = 0;
    
    if (*pw == 0)
    {
        return 0; // given pw is empty
    }
    
    result = memcmp(pw, pw_table[p_index], pw_len); // compare given pw with one in pw_table at p_index
    
    if (result == 0)
    {
        return 1; // password valid
    }
    
    return 0; // password invalid
}