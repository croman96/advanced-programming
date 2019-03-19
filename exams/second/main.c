/*----------------------------------------------------------------

*

* Programación avanzada: Examen 2

* Fecha: 19-Mar-2019

* Autor: A01700820 Carlos Román Rivera

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 100

int main(int argc, char* argv[]) {

  char buf[BUF_SIZE];       /* buffer */
  int parent_to_child[2];   /* pipeline */
  int child_to_parent[2];   /* pipeline */
  int pid;                  /* aux for creating processes */
  int i;                    /* aux for cycles */
  ssize_t aux;              /* aux for errors */

  if (argc != 1) {
    printf("usage: %s\n", argv[0]);
    return -1;
  }

  if (pipe(parent_to_child) == -1){
    printf("Error creating parent_to_child pipe.\n");
    return -1;
  }

  if (pipe(child_to_parent) == -1){
    printf("Error creating child_to_parent pipe.\n");
    return -1;
  }

  if ((pid = fork()) < 0) {

    printf("Error creating fork.\n");
    return -1;

  } else if (pid == 0) {

    printf("pid = %d, ppid = %d process starting...\n", getpid(), getppid());

    if (close(parent_to_child[1]) == -1) {
      printf("Error closing parent_to_child.\n");
      return -1;
    }

    if (close(child_to_parent[0]) == -1) {
      printf("Error closing child_to_parent.\n");
      return -1;
    }

    /* Read data from parent_to_child pipeline */
    while ((aux = read(parent_to_child[0], buf, BUF_SIZE)) > 0) {

      /* Convert tu uppercase */
      for (i = 0; i < aux; i++) {
        buf[i] = toupper((unsigned char) buf[i]);
      }

      /* Write data to child_to_parent pipeline */
      if (write(child_to_parent[1], buf, aux) != aux) {
        printf("Failed write to child_to_parent.\n");
        return -1;
      }

    }

    if (aux == -1) {
      printf("Error reading from parent_to_child pipeline.\n");
      return -1;
    }

  } else {

    printf("pid = %d, parent...\n", getpid());

    if (close(parent_to_child[0]) == -1) {
      printf("Error closing parent_to_child.\n");
      return -1;
    }

    if (close(child_to_parent[1]) == -1) {
      printf("Error closing child_to_parent.\n");
      return -1;
    }

    /* Read data from stdin */
    while ((aux = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {

      /* Write data on parent_to_child pipeline */
      if (write(parent_to_child[1], buf, aux) != aux) {
        printf("Failed writing on parent_to_child pipeline.\n");
        return -1;
      }

      /* Read data from child_to_parent pipeline */
      aux = read(child_to_parent[0], buf, BUF_SIZE);
      if (aux == -1) {
        printf("Error reading from child_to_parent pipeline.\n");
        return -1;
      }

      /* Print content to stdout */
      if (aux > 0) {
        if (write(STDOUT_FILENO, buf, aux) != aux) {
          printf("Error writing to stdout.\n");
          return -1;
        }
      }

    }

    if (aux == -1) {
      printf("Error reading from child_to_parent pipeline.\n");
      return -1;
    }

    exit(EXIT_SUCCESS);

  }

  return 0;

}
