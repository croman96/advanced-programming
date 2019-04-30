/*----------------------------------------------------------------

*

* Programación Avanzada: Manejo de Sockets

* Fecha: 26-Abr-2019

* Autor: A01700820 Carlos Román

*

*--------------------------------------------------------------*/

#include "header.h"
#include "string.h"

void serves_client(int nsfd, char* ipAddr) {
  char string[256];
  char ip[15];

	read(nsfd, string, sizeof(string));

  printf("%s: %s\n", ipAddr, string);

	write(nsfd, string, sizeof(string));

	close(nsfd);
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid;

	struct sockaddr_in server_info, client_info;
  struct in_addr ipAddr;

  char string[INET_ADDRSTRLEN];

	if ( (sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
		perror(program);
		exit(-1);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);

	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}

	listen(sfd, 1);

	while (1) {
		int len = sizeof(client_info);

    if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}

    ipAddr = client_info.sin_addr;
    inet_ntop(AF_INET, &ipAddr, string, INET_ADDRSTRLEN);

		serves_client(nsfd, string);
	}
}

int main(int argc, char* argv[]) {
	char ip[15];
	int port;

	strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;

	server(ip, port, argv[0]);

	return 0;
}
