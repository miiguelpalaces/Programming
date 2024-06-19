#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
    int fd = open("vacio.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1){
        perror("Error al abrir el archivo\n");
        return -1;
    }

    lseek(fd, 100, SEEK_SET);

    write(fd, "hola", 4);

}