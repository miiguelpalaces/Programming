#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>

#include <dirent.h>
#include <sys/types.h>

int main(int argc, char *argv[]){

    /*Control de argumentos*/
    if (argc != 3){
        printf("Usage: ./myishere <dir> <file>\n");
        return -1;
    }
    char *dirname = argv[1];
    char *filename = argv[2];

    /*Abrir el directorio con opendir*/
    DIR *dirp = opendir(dirname);

    if (dirp == NULL){
        printf("ERROR: failed to open %s directory. It may not exist.\n", dirname);
        return -1;
    }

    /*Leer cada una de los archivos del directorio (readdir) y compararlos con el dado*/
    struct dirent *direntp = readdir(dirp);

    int found = 0;
    while ((direntp = readdir(dirp)) != NULL) {
        if (strcmp(direntp->d_name, filename) == 0){
            printf("File %s is in directory %s\n", filename, dirname);
            found = 1;
            break;
        }
    }
    
    if (found == 0){
        printf("File %s is not in directory %s\n", filename, dirname);
    }
    
    /*Cerrar el directorio (closedir)*/
    if (closedir(dirp) == -1){
        printf("Error closing %s dir.\n", dirname);
        return -1;
    }

    return 0;
}