/*----------------------------------------------------------------

*

* Programación Avanzada: Manejo de Procesos

* Fecha: 1-Mar-2019

* Autor: A01700820 Carlos Román

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

void child_processes(int current, int levels, char* program){
  int pid, create;

  for (int i = 0; i < current; i++) {
    printf("  ");
  }

  printf("PPID = %i PID = %i LEVEL = %i\n", getppid(), getpid(), current);

  if (current < levels) {
    create = (current == 0) ? 1 : current * (current + 1) / current ;
  	for(int i = 0 ; i < create ; i++){
  		if ((pid = fork()) < 0) {
        printf("%s: error creating process.\n", program);
        exit(-1);
      } else if (pid == 0) {
        sleep(1);
        child_processes(current + 1, levels, program);
      } else {
        wait(NULL);
  			exit(0);
  		}
    }
  }

}

int main(int argc, char* argv[]) {
	int levels, current, processes, pid, rid;
  float read;
  levels = current = processes = 0;

  if (argc != 2) {
		fprintf(stderr, "usage: %s number\n", argv[0]);
		return -1;
	}

  read = atof(argv[1]);

  if (read < 1 || read != (int)read) {
		printf("%s: the parameter must be a positive integer number greater than zero.\n", argv[0]);
		return -1;
	}

  levels = (int)read;
  child_processes(0, levels, argv[0]);

	return 0;
}
