#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

// return length of a newline terminated string (length includes '\n' char)
int nlStrLen(char *str)
{
    int len = 0;
    
    while (*str != '\n')
    {
        len ++;
        str++;
    }
    
    return len;
}

// Reads from socket until a newline char is found
// newline terminated string is copied into *msg assuming *msg already has enough space allocated
// returns total length of message
int readNl(int socket, char *msg)
{
    int msgLen = 0, totalLen = 0;
    char buf[2]; //allocate a buffer to put partial message from socket into
    
    strcpy(msg, ""); // begin msg with empty string
    while ((msgLen = read(socket, buf, 1)) > 0) // continue reading from socket if there is more data
    {
        strncat(msg, buf, msgLen); // concatenate buf onto final mesage
        totalLen += msgLen;
        
        if (buf[msgLen-1] == '\n')
        {
            break; // end of message indicated by new line
        }
    }
    
    return totalLen;
}

// sends to socket the message length as 2-byte binary num and then send actual message after without a null terminating char
// returns -1 if write() failed; otherwise 0 for success
int writeLen(int socket, char *msg, int len)
{
    uint16_t network_byte_order;

    network_byte_order = htons(len);

    // send length of message
    if ((write(socket, &network_byte_order, sizeof(uint16_t))) < 0)
    {
        return -1;
    }

    // send message without null terminating char
    if ((write(socket, msg, len)) < 0)
    {
        return -1;
    }

    return 0;
}

// Reads from socket the message length then proceeds to read from socket and place in *msg
// until amount of bytes read is equal to given length
int readLen(int socket, char *msg, int readAmount)
{
    uint16_t msgLen;
    int rcvLen = 0, totalLen = 0;
    char buf[readAmount + 1];

    //Read in message length first
    read(socket, &msgLen, sizeof(uint16_t));
    msgLen = ntohs(msgLen); // convert from network byte order

    printf("pw len: %d\n", msgLen);

    // Read in message
    strcpy(msg, "");
    while ( (rcvLen = read(socket, buf, readAmount)) > 0)
    {
        totalLen += rcvLen;
        strncat(msg, buf, rcvLen);

        printf("len = %d\n", rcvLen);

        if (totalLen >= msgLen) // stop reading once total message length is greater/equal to received len
        {
            break;
        }
    }

    return msgLen;
}