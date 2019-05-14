/*----------------------------------------------------------------

*

* Programación avanzada: Examen Final

* Fecha: 10-May-2019

* Autor: A01700820 Carlos Román Rivera

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#include <float.h>

#define PATH_MAX  4096
#define NAME_MAX  255
#define SIZE      128

void merge_file(char *origin, char *destination, char* program) {

  int fd_in, fd_out;
  char buffer[SIZE];
	ssize_t nbytes;

	if ( (fd_in = open(origin, O_RDONLY)) < 0 ) {
		printf("%s cannot be read.\n", origin);
    return;
	} else {
    printf("merging %s...\n", origin);
  }

	if ( (fd_out = open(destination, O_WRONLY | O_APPEND, 0666)) < 0 ) {
		perror(program);
	}

	while ( (nbytes = read(fd_in, buffer, SIZE)) != 0 ) {
    write(fd_out, buffer, nbytes);
	}

	close(fd_in);
	close(fd_out);
}

void list(int cur_lvl, int max_lvl, char *origin, char *destination, char* program) {

	DIR* dir;
	struct dirent* direntry;
  struct stat info;

	char file_read[PATH_MAX + NAME_MAX + 1];

	if ( (dir = opendir(origin)) == NULL ) {
		perror(program);
		exit(-1);
	}

	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {

      sprintf(file_read, "%s/%s", origin, direntry->d_name);
      lstat(file_read, &info);

      if (S_ISDIR(info.st_mode)) {
        if (cur_lvl < max_lvl) {
          printf("Working in %s\n", direntry->d_name);
          list(cur_lvl + 1, max_lvl, file_read, destination, program);
        }
      } else {
        merge_file(file_read, destination, program);
      }
		}
	}

}

int main(int argc, char* argv[]) {

  int fd_out, max_level, current_level;

  char dir_name[NAME_MAX + 1];

  char origin[PATH_MAX + NAME_MAX + 1];
  char destination[PATH_MAX + NAME_MAX + 1];

	if (argc != 4) {
		fprintf(stderr, "usage: %s origin merge_file max_level\n", argv[0]);
		return -1;
	}

  max_level = atoi(argv[3]);

  if(max_level <= 1) {
    fprintf(stderr, "%s: the max_level must be a positive integer number greater than 1.\n", argv[0]);
    return -1;
  }

  current_level = 1;

  getcwd(dir_name, NAME_MAX);

  sprintf(origin, "%s/%s", dir_name, argv[1]);
  sprintf(destination, "%s/%s", dir_name, argv[2]);

  if ( (fd_out = open(destination, O_WRONLY | O_APPEND | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	} else {
    close(fd_out);
  }

  printf("Working in %s\n", argv[1]);
  list(current_level, max_level, origin, destination, argv[0]);

	return 0;
}
