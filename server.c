//SERVER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
}
