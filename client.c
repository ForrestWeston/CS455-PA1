//CLIENT
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
}
