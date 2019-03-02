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

void child_processes(int level, int max, char* program){
  int pid;

  for (int i = 0; i < level; i++) {
    printf("  ");
  }

	printf("PPID = %i PID = %i LEVEL = %i\n", getppid(), getpid(), level);

	if (level < max) {
    level += 1;
		for(int i = 0 ; i < level ; i++){
			if ((pid = fork()) < 0 ) {
        printf("%s: error creating process.\n", program);
        exit(-1);
      } else if (pid == 0) {
        sleep(1);
        child_processes(level, max, program);
      } else {
        wait(NULL);
				exit(0);
        if (level == max) {
          sleep(1);
        }
			}
		}
	}

}

int main(int argc, char* argv[]) {
	int current, processes, pid, rid;
  float levels;
  levels = current = processes = 0;

  if (argc != 2) {
		fprintf(stderr, "usage: %s number\n", argv[0]);
		return -1;
	}

  levels = atof(argv[1]);

  if (levels < 1 || levels != (int)levels) {
		printf("%s: the parameter must be a positive integer number greater than zero.\n", argv[0]);
		return -1;
	}

  child_processes(0, (int)levels, argv[0]);

	return 0;
}
