/*----------------------------------------------------------------

*

* Programación avanzada: Examen 3

* Fecha: 30-Abr-2019

* Autor: A01700820 Carlos Román Rivera

*

*--------------------------------------------------------------*/

#include "header.h"
#include <string.h>

void serves_client(int nsfd) {
  int i;
  uchar cmd;
  ulong id, val;

  FILE *file;
  CarPart part;

  for (i = 0; i < MAX_ITERATIONS; i++) {

    // Leer variables recibidas.
  	read(nsfd, &cmd, sizeof(uchar));
    read(nsfd, &id, sizeof(ulong));
    read(nsfd, &val, sizeof(ulong));

    // Fines ilustrativos.
    printf("Recibe - cmd %u - id = %lu - val = %lu\n", cmd, id, val);

    if (cmd == READ) {

      // Archivo en modo lectura.
      file = fopen ("inventory.dat", "r");

      if (file == NULL) {
        perror("file");
        exit(-1);
      }

      while(fread(&part, sizeof(struct part), 1, file)){
        if (part.id == id) {
          // Si es el indice solicitado, lee.
          val = part.quantity;
        }
      }

      // Mensje de retorno.
      cmd = SEND;

      // Cierra archivo.
      fclose (file);

      // Escribe los resultados de regreso.
      write(nsfd, &cmd, sizeof(uchar));
      write(nsfd, &val, sizeof(ulong));

    } else if (cmd == WRITE) {

      // Archivo en modo escritura.
      file = fopen ("inventory.dat", "w");

      if (file == NULL) {
        perror("file");
        exit(-1);
      }

      // Mensaje de retorno.
      cmd = OK;

      // Cierra archivo.
      fclose (file);

      // Escribe los resultados de regreso.
      write(nsfd, &cmd, sizeof(uchar));
      write(nsfd, &val, sizeof(ulong));

    }

  }

	close(nsfd);
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
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
    socklen_t len = sizeof(client_info);

		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}

		if ( (pid = fork()) < 0 ) {
			perror(program);
		} else if (pid == 0) {
			close(sfd);
			serves_client(nsfd);
			exit(0);
		} else {
			close(nsfd);
		}

	}
}

int main(int argc, char* argv[]) {
	char ip[15];
	int port;

	strcpy(ip, SERVER_IP);
	port = SERVER_PORT;

	server(ip, port, argv[0]);

	return 0;
}
