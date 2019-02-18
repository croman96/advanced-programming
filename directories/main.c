/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de Directorios

* Fecha: 12-Feb-2019

* Autor: A01206738 Carlos Román Rivera

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define PATH_MAX  4096
#define NAME_MAX  255

void get_info(char *name, char *directory, char* program) {
	int i;
	struct stat info;
	time_t last_m, last_a;

	// Se usa la estructura para convertir la fecha
	struct tm* timeinfo_m;
  struct tm* timeinfo_a;
	char date_m[24], date_a[24];
	char filename[PATH_MAX + NAME_MAX + 1];

	// Obtener info de ese archivo
	sprintf(filename, "%s/%s", directory, name);
	if (lstat(filename, &info) < 0) {
		perror(program);
		exit(-1);
	}

	// fechas
	last_m = info.st_mtime;
  last_a = info.st_atime;

	timeinfo_m = localtime(&last_m);
  timeinfo_a = localtime(&last_a);

	strftime(date_m, 24, "%a %b %d %X %Y", timeinfo_m);
  strftime(date_a, 24, "%a %b %d %X %Y", timeinfo_a);

  printf("Name: %s\n", name);
  printf("Path: %s\n", directory);
	printf("Last access: %s\n", date_a);
  printf("Last modification: %s\n", date_m);
  printf("\n");

}

void list(char *directory, int recursive, char* program) {
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];

	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}

	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			get_info(direntry->d_name, directory, program);
		}
	}

	if (recursive) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
			if (strcmp(direntry->d_name, ".") != 0 &&
				strcmp(direntry->d_name, "..") != 0) {
				sprintf(filename, "%s/%s", directory, direntry->d_name);
				lstat(filename, &info);
				if (S_ISDIR(info.st_mode)) {
					list(filename, recursive, program);
				}
			}
		}
	}

	printf("\n");
}


int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	int recursive;

	if (argc < 3 || argc > 4) {
		printf("usage: %s str directory [-r] \n", argv[0]);
		return -1;
	}

	recursive = 0;

	if (argc == 4) {
		if (strcmp(argv[3], "-r") != 0) {
      printf("usage: %s str directory [-r] \n", argv[0]);
  		return -1;
		} else{
      recursive = 1;
    }
	}

  getcwd(dir_name, NAME_MAX);
	directory = dir_name;

  list(directory, recursive, argv[0]);

  return 0;
}
