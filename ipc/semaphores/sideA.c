/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos

* Fecha: 29-Mar-2019

* Autor: A01700820 Carlos Roman

* Autor: A00000000 Marco Luna

*

*--------------------------------------------------------------*/

#include "header.h"

void sideA(char* program) {

	int semid;
	key_t key;

	if ( (key = ftok("/dev/null", 65)) == (key_t) - 1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}

  // Wait for resource to be available.

	printf("%i is trying to cross.\n", getpid());
	mutex_wait(semid, MUTEX);

  if (semctl(semid, SIDEB, GETVAL, 0) > 0) {
    printf("Bridge is unavailable.\n"); // Bridge unavailable
  } else {
    sem_signal(semid, SIDEA, 1);        // Indicate people going from A to B
  	printf("%i can go from A to B.\n", getpid());
    sem_signal(semid, BRIDGE, 1);       // Make resource unavailable from B to A
  }

  sleep(SLEEP);

  sem_wait(semid, SIDEA, 1);
  sem_wait(semid, BRIDGE, 1);

	mutex_signal(semid, MUTEX);  // Notify that I have finished. Free resources.

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

  // Create any given number of farmers (processes) wanting to cross.

	for (i = 0; i < farmers; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			sideA(argv[0]);
		}
	}

	return 0;

}
