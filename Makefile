all: server client

server: server.c helpme.c
		gcc -Wall -Werror -g server.c helpme.c -o server

client: client.c helpme.c
		gcc -Wall -Werror -g client.c helpme.c -o client
