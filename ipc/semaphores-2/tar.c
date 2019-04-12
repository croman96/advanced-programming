/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos 2 (El oso y las abejas)

* Fecha: 12-Abr-2019

* Autor: A01700820 Carlos Roman

*

*--------------------------------------------------------------*/

#include "header.h"

int main(int argc, char* argv[]) {
	int size = 0, semid;
	key_t key;

	if (argc != 2) {
		printf("usage: %s size\n", argv[0]);
		return -1;
	}

	size = atoi(argv[1]);

	if (size < 1) {
		printf("%s: The size must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}

	if ( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	semctl(semid, MUTEX_BEAR,	SETVAL, 0);			// Oso comienza dormido.
	semctl(semid, COUNT_ITEM, SETVAL, 0);			// Tarro comienza vacio.
	semctl(semid, MUTEX_BEES, SETVAL, 1);			// Abejas comienzan produciendo.
	semctl(semid, COUNT_MAX, 	SETVAL, size);	// Inicializa capacidad del tarro.

	printf("Semaphore:\t%i\n", semid);

	printf("Mutex Bear:\t%i\n", semctl(semid, MUTEX_BEAR, GETVAL, 0));
	printf("Count Item:\t%i\n", semctl(semid, COUNT_ITEM, GETVAL, 0));
	printf("Mutex Bees:\t%i\n",	semctl(semid, MUTEX_BEES, GETVAL, 0));
	printf("Count Max:\t%i\n",  semctl(semid, COUNT_MAX,  GETVAL, 0));

	return 0;
}
