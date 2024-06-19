//SSOO-P3 23/24

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

//To create a queue
queue* queue_init(int size)
{   
    // Control de parametros. Es necesario que size sea positivo
    if (size <= 0){
        perror("Queue size must be greater than 0");
        return NULL;
    }

    queue * q = (queue *)malloc(sizeof(queue)); // Se reserva espacio en memoria para el buffer circular

    q->orders = (struct element *)malloc(size * sizeof(struct element)); // Se reserva suficiente espacio en memoria para las órdenes
    q->max_size = size; // Nº máximo de órdenes almacenables en el buffer circular. CONSTANTE
    q->size = 0; // Nº de órdenes actuales en el buffer circular
    q->head = 0; // Posición de la orden inicial (la próxima en ser atendida)
    q->tail = -1; // Posición de la orden final (la ultima añadida)

    // Inicializar todas las órdenes a -1, para indicar que están vacías
    for (int i = 0; i < size; i++){
        q->orders[i].product_id = -1;
        q->orders[i].op = -1;
        q->orders[i].units = -1;
    }
    
    return q; // Devolver el buffer circular
}

// To Enqueue an element
int queue_put(queue *q, struct element* x)
{   
    // Si el buffer esta lleno, no se pueden añadir más elementos
    if (queue_full(q)){
        perror("Error: Queue is full, cannot put more elements");
        return -1;
    }

    // Añadir elemento al buffer circular
    if (q->tail == q->max_size - 1) q->tail = 0; // Si tail ha dado la vuelva al buffer circular, darle el valor 0
    else q->tail++; // En caso contrario, simplemente aumentar en 1 su valor
    q->orders[q->tail] = *x; // Anadir la nueva orden al buffer
    q->size++; // Aumentar el tamaño actual

    return 0;
}

// To Dequeue an element.
struct element* queue_get(queue *q)
{   
    // Si el buffer esta vacio, no se pueden sacar más elementos
    if (queue_empty(q)){
        perror("Error: Queue is empty, cannot get elements");
        return NULL;
    }

    // Sacar elemento del buffer circular
    struct element *new_element = (struct element *)malloc(sizeof(struct element)); // Reservar espacio en memoria para el nuevo elemento
    memcpy(new_element, &q->orders[q->head], sizeof(struct element)); // Copiar el elemento a una nueva dirección de memoria

    // Limpiar la ordendiente a la que apunta head, puesto que ya hemos procesado esa orden
    q->orders[q->head].product_id = -1;
    q->orders[q->head].op = -1;
    q->orders[q->head].units = -1;

    // Actualizar head
    if (q->head == q->max_size - 1) q->head = 0; // Si head ha dado la vuelta al buffer circular, darle el valor 0
    else q->head++; // En caso contrario, simplemente aumentar en 1 su valor

    q->size--; // Disminuir el tamaño actual

    return new_element; // Devolver la orden extraida
}

//To check queue state
int queue_empty(queue *q)
{   
    // La cola está vacía si el tamaño actual es 0
    if (q->size == 0) return 1;
    else return 0;
}

int queue_full(queue *q)
{   
    // La cola está llena si el tamaño actual es igual al tamaño máximo
    if (q->size == q->max_size) return 1;
    else return 0;
}

//To destroy the queue and free the resources
int queue_destroy(queue *q)
{      
    // Se destruye la cola y se liberan los recursos
    free(q->orders);
    free(q);
    
    return 0;
}