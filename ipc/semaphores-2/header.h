/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos 2 (El oso y las abejas)

* Fecha: 12-Abr-2019

* Autor: A01700820 Carlos Roman

*

*--------------------------------------------------------------*/

#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define	MUTEX_BEAR 	0		// Oso dormido o despierto.
#define COUNT_ITEM	1		// Cuantas porciones de miel hay en el tarro.
#define	MUTEX_BEES	2		// Controla acceso al tarro.
#define COUNT_MAX		3		// Capacidad del tarro.

int mutex_wait(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

#endif
