/*----------------------------------------------------------------

*

* Programación avanzada: Cifrado de un archivo

* Fecha: 1-Feb-2019

* Autor: A01700820 Carlos Román Rivera

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define SIZE 128

int isNumber(char number[]) {
  int num, aux = 0;

  if (number[0] == '-') {
    return 0;
  }

  for (int i = 0; number[i] != 0; i++) {
    if (!isdigit(number[i])) {
      return 0;
    } else {
      aux = (aux * 10) + (number[i] - '0');
    }
  }

  return aux;
}

int main(int argc, char* argv[]) {

  int fd_in, fd_out;
  char buffer[SIZE], buffer2[SIZE], auxc;
	ssize_t nbytes;

	if (argc != 4) {
		fprintf(stderr, "usage: %s number origin destination\n", argv[0]);
		return -1;
	}

  int rotate = isNumber(argv[1]);

  if(!rotate) {
    fprintf(stderr, "%s: the first parameter must be a positive integer number.\n", argv[0]);
    return -1;
  }

	if ( (fd_in = open(argv[2], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	while ( (nbytes = read(fd_in, buffer, SIZE)) != 0 ) {
    if (nbytes == SIZE) {
      for(int i = 0 ; i < rotate ; i++){
        auxc = buffer[0];
        for(int j = 1 ; j < SIZE; j++){
          buffer[j-1] = buffer[j];
        }
        buffer[SIZE-1] = auxc;
      }
    }
    write(fd_out, buffer, nbytes);
	}

  printf("Done.\n");

	close(fd_in);
	close(fd_out);

	return 0;
}
