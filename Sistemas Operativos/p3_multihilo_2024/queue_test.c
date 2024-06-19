#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

void print_queue(queue *q){
    /*
    Imprime el buffer circular con cada operación entre head y tail
    Uso único para debugging
    */
    printf("QUEUE: [");

    for (int i = 0; i < q->max_size; i++){
        if (q->orders[i].product_id != -1){
            printf("{%d, %d, %d}, ", q->orders[i].product_id, q->orders[i].op, q->orders[i].units);
        }
        else{
            printf("NULL, ");
        }
    }
    printf("]\n");
}


int main(){

    queue *q = queue_init(3);
    queue_destroy(q);
    
    // Añadir un elemento a una cola ya llena
    queue_destroy(q);
    
}