/*----------------------------------------------------------------

*

* Programación Avanzada: Semaforos

* Fecha: 29-Mar-2019

* Autor: A01700820 Carlos Roman

* Autor: A00000000 Marco Luna

*

*--------------------------------------------------------------*/

#include "header.h"

int main(int argc, char *argv[]) {

  int semid;
  key_t key;

  if( (key = ftok("/dev/null", 65)) == (key_t) - 1) {
    perror(argv[0]);
    return -1;
  }

  if( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
    perror(argv[0]);
    return -1;
  }

  semctl(semid, BRIDGE, SETVAL, 0);   // Represent bridge.
  semctl(semid, MUTEX,  SETVAL, 1);   // Control access to resources.
  semctl(semid, SIDEA,  SETVAL, 0);   // Represent people going from A to B.
  semctl(semid, SIDEB,  SETVAL, 0);   // Represent people going from B to A.

  return 0;

}
