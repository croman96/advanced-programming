/*----------------------------------------------------------------

*

* Programación avanzada: Examen 1

* Fecha: 15-Feb-2019

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

#define SIZE 1024
#define PATH_MAX  4096
#define NAME_MAX  255

int kb_limit;

int is_directory(char *directory) {
	DIR* dir;
	if ( (dir = opendir(directory)) == NULL ) {
		perror(directory);
    exit(-1);
	}
  return 1;
}

int is_number(char *number) {
  int num, aux = 0;
  if (number[0] == '-') {
    printf("%s must be a positive number greater than zero.\n", number);
    exit(-1);
  }
  for (int i = 0; number[i] != 0; i++) {
    if (!isdigit(number[i])) {
      printf("%s must be a positive number greater than zero.\n", number);
      exit(-1);
    } else {
      aux = (aux * 10) + (number[i] - '0');
    }
  }
  return aux;
}

int copy_file(char *path, char *path_2, char *origin_directory, char *file, char *destination){

  int fd_in, fd_out;
  char buffer[SIZE], move_from[500], move_to[500];
	ssize_t nbytes;

  if ( (fd_in = open(path, O_RDONLY)) < 0 ) {
		perror(file);
		return -1;
	}

	if ( (fd_out = open(path_2, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(file);
		return -1;
	}

	while ( (nbytes = read(fd_in, buffer, SIZE)) != 0 ) {
    write(fd_out, buffer, nbytes);
	}

	close(fd_in);
	close(fd_out);

  return 1;
}

void get_info(char *name, char *directory, char* program){
	struct stat info;
  int size, size_in_kb;
	char filename[PATH_MAX + NAME_MAX + 1];
  char path[PATH_MAX + NAME_MAX + 1];

	sprintf(filename, "%s/%s", directory, name);
	if (lstat(filename, &info) < 0) {
		perror(program);
		exit(-1);
	}

  size = info.st_size;
  size_in_kb = size * 0.001 ;
  getcwd(path, NAME_MAX);

  printf("Name: %s\n", name);
  printf("Size: %d\n", size_in_kb);
  printf("Directory: %s\n", directory);
	printf("Path: %s\n", path);

  if(size_in_kb > kb_limit){
    printf("File is moved to greater.\n");
    copy_file(path, path, directory, name, "greater");
  }else{
    printf("File is moved to lesser.\n");
    copy_file(path, path, directory, name, "lesser");
  }

  printf("\n");

}

void list(char *directory, char* program) {
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];

	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}

	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			get_info(direntry->d_name, directory, program);
		}
	}

	rewinddir(dir);
	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			lstat(filename, &info);
			if (S_ISDIR(info.st_mode)) {
				list(filename, program);
			}
		}
	}

	printf("\n");
}

int main(int argc, char* argv[]){

  // Validate number of received parameters.
	if (argc != 5) {
		fprintf(stderr, "usage: %s base lesser greater N\n", argv[0]);
		return -1;
	}

  // Check if first 3 parameters are valid directories.
  is_directory(argv[1]);
  is_directory(argv[2]);
  is_directory(argv[3]);

  // Check if N is a positive integer number.
  kb_limit = is_number(argv[4]);

  list(argv[1], argv[0]);

	return 0;
}
