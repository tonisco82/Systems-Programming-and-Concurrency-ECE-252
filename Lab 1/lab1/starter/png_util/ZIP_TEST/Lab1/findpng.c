#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab_png.h"

void check_directory(char* d_name, int *png_count) {
	DIR* p_dir;
	struct dirent *p_dirent;

	/*if directory cannot be opened, simply return*/
	if ((p_dir = opendir(d_name)) == NULL) {
		return;
	}

	/*keep reading directory contents*/
	while ((p_dirent = readdir(p_dir)) != NULL) {
		char *str_path = p_dirent->d_name;

		if (str_path == NULL) {
			return;
		} else {
			if (p_dirent->d_type == 4) {
                                /*check directories are not "." and ".."*/
                                if (strcmp(str_path, ".") != 0 && strcmp(str_path, "..") != 0) {
                                	/*recursively check subfolders*/
					char dir_name[256];
					sprintf(dir_name, "%s/%s", d_name, str_path);
					check_directory(dir_name, png_count);
				}
                        }
                        /*else if potential png candidate found*/
                        if (p_dirent->d_type == 8) {
                                /*check first if valid png*/
				char file_name[256];
				sprintf(file_name, "%s/%s", d_name, str_path);
				FILE* fp = fopen(file_name, "r");
				U8 buffer[8]; 
				fread(buffer, 8, 1, fp);
				if (is_png(buffer, 0)) {
					fclose(fp);
					continue;
				}  
                                printf("%s/%s\n", d_name, str_path);
                                png_count++;
				fclose(fp);
                        }
		}
	}

	/*close directory*/
	if (closedir(p_dir) != 0) {
                perror("closedir");
                exit(3);
        }

}

int main(int argc, char** argv) {

	struct dirent *p_dirent;

	if (argc == 1) {
		fprintf(stderr, "Usage: %s <directory name>\n", argv[0]);
		exit(1);
	}

	int png_count = 0;

	check_directory(argv[1], &png_count);

	/*if empty search result*/
	if (png_count) {
		printf("findpng: No PNG file found\n");
	}

	return 0;
}
