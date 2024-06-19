#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 1024 * 1024 /*1 MB elementos almacenados en el buffer*/

int main(int argc, char *argv[]) {

    /*Control de paso de parámetros*/
    if (argc != 2) {
        printf("Usage: ./mywc <file>\n");
        return -1;
    }

    /*Abrir el fichero*/
    char *filename = argv[1];
    int fd = open(filename, O_RDONLY);

    if (fd < 0){
        printf("Error opening %s file. There may not be a file with such name.\n", filename);
        return -1;
    }

    /*Leer los contenidos del fichero*/
    char buffer[SIZE];
    int nlines = 0, nwords = 0, nbytes = 0;
    int inword = 0;
    int nreads = -1;

    while ((nreads = read(fd, buffer, SIZE)) != 0){
        
        /*Control de función read + cierre del fichero abierto*/
        if (nreads < 0){
            printf("Error with read system call\n");

            if (close(fd) == -1){
                printf("Error closing %s file.\n", filename);
                return -1;
            }
            return -1;
        }

        for (int i = 0; i < nreads; i++){

            /*lines*/
            if (buffer[i] == '\n') {
                nlines ++;
            }

            /*words*/
            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r'){
                inword = 0;
            } else if (inword == 0){
                inword = 1;
                nwords ++;
            }

            /*bytes*/
            nbytes ++;
        }
    }

    /*Mostrar resultados por consola*/
    printf("%d %d %d %s\n", nlines, nwords, nbytes, filename);

    /*Cerrar fichero*/
    if (close(fd) == -1){
        printf("Error closing %s file.\n", filename);
        return -1;
    }

    return 0;
}