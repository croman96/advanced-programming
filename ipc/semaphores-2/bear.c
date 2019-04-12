/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos 2 (El oso y las abejas)

* Fecha: 12-Abr-2019

* Autor: A01700820 Carlos Roman

*

*--------------------------------------------------------------*/

#include "header.h"
#include <time.h>

#define BEARS 1

void a_consumer(char* program) {
	int semid, i, k, portions;
	key_t key;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror(program);
		exit(-1);
	}

	while (1) {
		printf("Bear %i is sleeping.\n", getpid());
		mutex_wait(semid, MUTEX_BEAR);											// Espera a despiertar.
		printf("Bear %i accessing the tar.\n", getpid());

		portions = semctl(semid, COUNT_ITEM, GETVAL, 0);

		while (portions) {
			printf("Bear %i trying to eat - portions available = %i\n", getpid(), portions);
			sem_wait(semid, COUNT_ITEM, 1);										// Come una porcion.
			printf("Bear %i took a portion.\n", getpid());
			portions--;
			sleep(1);
		}

		mutex_signal(semid, MUTEX_BEES);										// Libera el tarro.
		printf("Bear %i is going to sleep.\n", getpid());
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int semid, i, pid;
	key_t key;

	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}

	for (i = 0; i < BEARS; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_consumer(argv[0]);
		} else {
		}
	}
	return 0;
}
