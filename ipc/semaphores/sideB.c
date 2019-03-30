/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos

* Fecha: 29-Mar-2019

* Autor: A01700820 Carlos Roman

* Autor: A00000000 Marco Luna

*

*--------------------------------------------------------------*/

#include "header.h"

void sideB(char* program) {

	int semid;
	key_t key;

  // Exact same use as sideA.c but in reverse order.

	if ( (key = ftok("/dev/null", 65)) == (key_t) - 1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror(program);
		exit(-1);
	}

	printf("%i is trying to cross.\n", getpid());
	mutex_wait(semid, MUTEX);

  if (semctl(semid, SIDEA, GETVAL, 0) > 0) {
    printf("Bridge is unavailable.\n");
  } else {
    sem_signal(semid, SIDEB, 1);
  	printf("%i can go from B to A.\n", getpid());
    sem_signal(semid, BRIDGE, 1);
  }

  sleep(SLEEP);

  sem_wait(semid, SIDEB, 1);
  sem_wait(semid, BRIDGE, 1);

	mutex_signal(semid, MUTEX);

	exit(0);

}

int main(int argc, char* argv[]) {

	int pid, farmers, i;
	key_t key;

	if (argc != 2) {
		printf("usage: %s farmers\n", argv[0]);
		return -1;
	}

	farmers = atoi(argv[1]);

	if (farmers < 1) {
		printf("%s: farmers must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < farmers; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			sideB(argv[0]);
		}
	}

	return 0;

}
