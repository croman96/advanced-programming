/*----------------------------------------------------------------

*

* Programación Avanzada: Manejo de Senales

* Fecha: 12-Mar-2019

* Autor: A01700820 Carlos Román

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

void signal_handler(int signal) {
	switch (signal){
		case SIGHUP:
			printf("Punto de retorno 1\n");
			printf("Tabla de cuadrados\n");
			printf("Número\tPow\n");
			printf("1\t1\n");
			printf("2\t4\n");
			printf("3\t9\n");
			printf("4\t16\n");
			printf("5\t25\n");
      printf("\nAbre una nueva terminal para enviar instrucciones.\n");
		case SIGINT:
			printf("Punto de retorno 2:\n");
			printf("Número\tSqrt\n");
			printf("1\t1.00000\n");
			printf("2\t1.41421\n");
			printf("3\t1.73205\n");
			printf("4\t2.00000\n");
			printf("5\t2.23607\n");
      printf("\nAbre una nueva terminal para enviar instrucciones.\n");
		case SIGQUIT:
			printf("Punto de retorno 3:\n");
			printf("Número\tLogaritmo\n");
			printf("1\t0.00000\n");
			printf("2\t0.69315\n");
			printf("3\t1.09861\n");
			printf("4\t1.38629\n");
			printf("5\t1.60944\n");
      printf("\nAbre una nueva terminal para enviar instrucciones.\n");
			break;
		default:
			printf("Senal invalida!\n");
			break;
	}
}

int main(int argc, char* argv[]) {

	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	if (argc != 1) {
		printf("usage: jumps\n");
		return -1;
	} else {
		printf("Punto de retorno 1\n");
		printf("Número\tPow\n");
		printf("1\t1\n");
		printf("2\t4\n");
		printf("3\t9\n");
		printf("4\t16\n");
		printf("5\t25\n");
		printf("Punto de retorno 2:\n");
		printf("Número\tSqrt\n");
		printf("1\t1.00000\n");
		printf("2\t1.41421\n");
		printf("3\t1.73205\n");
		printf("4\t2.00000\n");
		printf("5\t2.23607\n");
		printf("Punto de retorno 3:\n");
		printf("Número\tLogaritmo\n");
		printf("1\t0.00000\n");
		printf("2\t0.69315\n");
		printf("3\t1.09861\n");
		printf("4\t1.38629\n");
		printf("5\t1.60944\n");
		printf("PID = %i\n", getpid());
    printf("\nAbre una nueva terminal para enviar instrucciones.\n");
	}

	while(1){
		sleep(10);
	}

	return 0;
}
