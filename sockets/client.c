/*----------------------------------------------------------------

*

* Programación Avanzada: Manejo de Sockets

* Fecha: 26-Abr-2019

* Autor: A01700820 Carlos Román

*

*--------------------------------------------------------------*/

#include "header.h"
#include "string.h"

int main(int argc, char* argv[]) {
  int sfd;
  int size = 256;
  char string[size];
	struct sockaddr_in server_info;

	if (argc != 2) {
	    printf("usage: %s ip\n", argv[0]);
	    return -1;
	}

	if ( (sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(DEFAULT_PORT);

  if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	printf("Text to print on server: ");
	fgets(string, size, stdin);
  strtok(string, "\n");

	write(sfd, string, sizeof(string));

	read(sfd, string, sizeof(string));
  printf("Text printed on server: %s\n", string);

	close(sfd);

	return 0;
}
