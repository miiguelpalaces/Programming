#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <limits.h>
#include <string.h>

#include <dirent.h>
#include <sys/types.h>
#include <linux/limits.h>

int main(int argc, char *argv[]){

    /*Control de argumentos*/
    if (argc > 2){
        printf("Usage: ./myls [dir]\n");
        return -1;
    }  

    /*En caso de que el usuario quiera listar otro dir, encontrar su path*/
    DIR *dirp = NULL;
    char path_buf[PATH_MAX];
    if (argc == 1){
        /*Caso en el que no se pasa ningún otro directorio*/
        getcwd(path_buf, PATH_MAX);
        
        if (path_buf == NULL){
            printf("ERROR: getcwd failed to obtain path\n");
            return -1;
        }

    } else {
        /*Caso en el que se pasa otro dir, tanto su path absoluto como relativo*/
        strcpy(path_buf, argv[1]);
    }

    /*Abrir el directorio con opendir*/
    dirp = opendir(path_buf);

    if (dirp == NULL){
        printf("ERROR: failed to open %s directory. It may not exist.\n", path_buf);
        return -1;
    }

    /*Leer cada una de las entradas del directorio (readdir) e imprimirlas*/
    struct dirent *direntp = NULL;

    while ((direntp = readdir(dirp)) != NULL) {
        printf("%s\n", direntp->d_name);
    }

    /*Cerrar el directorio (closedir)*/
    if (closedir(dirp) == -1){
        printf("ERROR: failed to close %s directory.\n", path_buf);
        return -1;
    }

    return 0;
}