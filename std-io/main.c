/*----------------------------------------------------------------

*

* Programacion avanzada: Funciones Estandar de E/S

* Fecha: 22-Feb-2019

* Autor: A01700820 Carlos Roman Rivera

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

void normalize(char *origin, char *destination, char* program) {

  // File manipulation.
  FILE* file_read;
  FILE* file_write;

  // Auxiliary variables for normalization.
	int aux, total, i;
  float num, max, min, x;

  // Open origin directory, otherwise throw error.
  file_read = fopen(origin, "r");
	if (!file_read) {
		perror(program);
		exit(-1);
	}

  // Open destination directory, otherwise throw error.
  file_write = fopen(destination, "w");
	if (!file_read) {
		perror(program);
		exit(-1);
	}

  // Iterate through each line of the file. Discard first two elements.
	while (fscanf(file_read, "%i,%i", &aux, &total) > 0) {
    fprintf(file_write, "%i,%i", aux, total);

    // Set auxiliary variables to initial values.
		total = total * total;
    max = FLT_MIN;
    min = FLT_MAX;
    float arr[total + 1];

    // Iterate through input file to get min and max values.
		for (i = 0; i < total; i++) {
			fscanf(file_read, ",%f", &num);
      arr[i] = num;
      if(num < min){
        min = num;
      } else if (num > max) {
        max = num;
      }
		}

    // Update values.
    for (i = 0; i < total; i++) {
      x = (arr[i] - min) / (max - min);
      fprintf(file_write, ",%.4f", x);
		}

    // Reset variables and insert newline to output file.
    total = 0;
    fprintf(file_write, "\n");
	}

	fclose(file_read);
  fclose(file_write);
}

void list(char *origin, char *destination, char* program) {

  // Directory manipulation.
	DIR* dir;
  DIR* dir2;
	struct dirent* direntry;

  // File names and paths.
	char file_read[PATH_MAX + NAME_MAX + 1];
  char file_write[PATH_MAX + NAME_MAX + 1];

	if ( (dir = opendir(origin)) == NULL ) {
		perror(program);
		exit(-1);
	}

  if ( (dir2 = opendir(destination)) == NULL ) {
		perror(program);
		exit(-1);
	}

  // Iterate through the given directory.
	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {
      // Get absolute path for input and output files.
			sprintf(file_read, "%s/%s", origin, direntry->d_name);
      sprintf(file_write, "%s/%s", destination, direntry->d_name);
      // Normalize data from input file and write in output file.
			normalize(file_read, file_write, program);
		}
	}

	printf("Done.\n");
}

int main(int argc, char* argv[]) {

  // Directory manipulation.
  char dir_name[NAME_MAX + 1];
  char origin[PATH_MAX + NAME_MAX + 1];
  char destination[PATH_MAX + NAME_MAX + 1];

  // Check correct usage.
	if (argc != 3) {
		fprintf(stderr, "usage: %s input_directory output_directory\n", argv[0]);
		return -1;
	}

  getcwd(dir_name, NAME_MAX);

  // Build absolute path for origin and destination directories.
  sprintf(origin, "%s/%s", dir_name, argv[1]);
  sprintf(destination, "%s/%s", dir_name, argv[2]);

  // Iterate through the origin directory.
  list(origin, destination, argv[0]);

	return 0;
}
