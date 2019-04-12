/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos 2 (El oso y las abejas)

* Fecha: 12-Abr-2019

* Autor: A01700820 Carlos Roman

*

*--------------------------------------------------------------*/

#include "header.h"
#include <time.h>

void a_producer(char* program) {
	int semid, i, k, current, missing, max;
	key_t key;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror(program);
		exit(-1);
	}

	srand(getpid());

	while (1) {
		printf("Producer %i accessing the buffer.\n", getpid());

		i = (rand() % 3) + 1;

		printf("Producer %i trying to put %i product(s).\n", getpid(), i);

		mutex_wait(semid, MUTEX_BEES);						// Obtiene el tarro.

		current = semctl(semid, COUNT_ITEM, GETVAL, 0);
		max = semctl(semid, COUNT_MAX, GETVAL, 0);
		missing = max - current;

		if (i < missing) {												// No lleno el tarro.
			printf("Producer %i put %i product(s) - %i free spaces\n", getpid(), i, missing - i);
			sem_signal(semid, COUNT_ITEM, i);				// Inserta porciones producidas.
			mutex_signal(semid, MUTEX_BEES);				// Libera el tarro.
		} else {
			printf("Producer %i put %i product(s) - %i free spaces\n", getpid(), missing, 0);
			sem_signal(semid, COUNT_ITEM, missing);	// Llena el tarro.
			mutex_signal(semid, MUTEX_BEAR);				// Despierta al oso.
			printf("Producer %i is going to sleep.\n", getpid());
			printf("Bees are waiting.\n");
		}

		sleep((rand() % 5) + 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, semid, i, pid;
	key_t key;

	if (argc != 2) {
		printf("usage: %s amount\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount < 1) {
		printf("%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_producer(argv[0]);
		} else {
		}
	}
	return 0;
}
