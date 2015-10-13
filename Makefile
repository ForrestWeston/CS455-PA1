server:
		gcc -Wall -Werror -g server.c helpme.c -o server

client:
		gcc -Wall -Werror -g client.c helpme.c -o client
