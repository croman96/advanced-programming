/*----------------------------------------------------------------

*

* Programacion avanzada: Primer Proyecto

* Fecha: 3-Mar-2019

* Autor: A01700820 Carlos Roman Rivera

*

*--------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <limits.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

int FILESIZE = PATH_MAX + NAME_MAX + 1;

void rotation(char* directory, char* filename, int rotations, int timeout, char* program) {

	DIR* dir;
	FILE* file;

	struct dirent** direntry;
	struct stat info;

  char old_file[FILESIZE], new_file[FILESIZE];
  char* split_name[2];
  char* token;
  char* name;
  int dir_files, filename_length;
  int aux, i, j;

	// Check if we can open the directory.
	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}

	lstat(directory, &info);

	// Check if we have the right permissions in the working directory.
	if (info.st_mode & S_IRUSR && info.st_mode & S_IWUSR) {
		// Continue.
	} else {
		printf("%s: no read and write permission for %s\n", program, directory);
    exit(-1);
	}

	// Check if the initial .log file exists.
  sprintf(old_file, "%s/%s.log", directory, filename);
  file = fopen(old_file, "r");
  if (!file) {
    perror(program);
    exit(-1);
  }
  fclose(file);

  filename_length = strlen(filename);

  while (1) {
		// Sleep until timeout.
    sleep(timeout);

		// Begin rotation.
    printf("\nTimeout\n");

		// Current files in directory, sorted in ascending order.
   	dir_files = scandir(directory, &direntry, NULL, alphasort) - 1;

		// Iterate through files.
    while (dir_files >= 0) {

      name = direntry[dir_files]->d_name;

			// If current file has given filename, we must rename it.
      if (strncmp(name, filename, filename_length) == 0) {

        j = 0;
        token = strtok(name, ".");
        while (token != NULL) {
            split_name[j] = token;
            token = strtok(NULL, ".");
            j++;
        }

				// split_name[0]	->	filename
				// split_name[1]	->	extension

        sprintf(old_file, "%s/%s.%s", directory, split_name[0], split_name[1]);

				// If it is not the .log file, work with it.
        if (strncmp(split_name[1],"log",3) != 0) {
          aux = atoi(split_name[1]);
					// If current file exceeds number of rotations, delete. Else, rename.
          if (aux >= rotations) {
            remove(old_file);
            printf("Delete %s\n", old_file);
          } else {
            sprintf(new_file, "%s/%s.%d", directory, filename, aux + 1);
            rename(old_file, new_file);
            printf("Rename %s to %s\n", old_file, new_file);
          }
        }
      }
			// Go to next file.
      dir_files--;
    }

		// Rename .log file to .1
    sprintf(old_file, "%s/%s.log", directory, filename);
    sprintf(new_file, "%s/%s.1", directory, filename);
    rename(old_file, new_file);
    printf("Rename %s to %s\n", old_file, new_file);

		// Create new .log file.
    sprintf(new_file, "%s/%s.log", directory, filename);
    open(new_file, O_WRONLY | O_TRUNC | O_CREAT, 777);
    printf("Create %s\n", new_file);
  }

}

int main(int argc, char* argv[]) {

	char *directory, *filename, *program;
	int rotations, timeout;

	if (argc > 5 || argc < 5) {
		printf("usage: %s working_directory log_filename number_of_files timeout\n", argv[0]);
		return -1;
	}

  program = argv[0];
  directory = argv[1];
  filename = argv[2];
	rotations = atoi(argv[3]);
	timeout = atoi(argv[4]);

	if (rotations < 1) {
		printf("%s: number_of_files must be a positive integer greater than zero\n", argv[0]);
		return -1;
	}

	if (timeout < 1) {
		printf("%s: timeout must be a positive integer greater than zero\n", argv[0]);
		return -1;
	}

	rotation(directory, filename, rotations, timeout, program);

	return 0;
}
