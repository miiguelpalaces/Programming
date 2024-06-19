//SSOO-P3 23/24

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

#define FILE_LINE_MAX_SIZE 50
#define FILE_NAME_MAX_SIZE 50
#define MAX_NUMBER_ORDERS 200

#define PRODUCT_ID 0
#define BUYING_COST 1
#define SELLING_PRICE 2

// Estructura que contiene el rango de operaciones que le corresponde a cada productor. Se utiliza para pasarse como argumento
typedef struct {
    int start;
    int end;
}range;

// Tabla que relaciona el id del un producto (primera columna), con su coste de compra (segunda columna) y su precio de venta (tercera columna)
int price_table[5][3] = {
    {1, 2, 3},
    {2, 5, 10},
    {3, 15, 20},
    {4, 25, 40},
    {5, 100, 125}
};

// Declarar struct operaciones y cola circular de operaciones
struct element *orders;
queue *or_queue;

// Declarar mutex y variables condicionales
pthread_mutex_t mutex; // Mutex que se usa para controlar el acceso a las variables compartidas
pthread_cond_t not_empty; // Variable condicional que se usa para indicar que la cola no está vacía
pthread_cond_t not_full; // Variable condicional que se usa para indicar que la cola no está llena


// Las siguientes variables se utilizan por varios procesos, por lo que se ha considerado que deben ser globales
int num_orders = -1; // Nº de órdenes a procesar
int orders_consumed = 0; // Nº de órdenes consumidas. Se usa para saber cuando deben de terminar los procesos consumidores
int profits = 0; // Beneficios
int product_stock [5] = {0, 0, 0, 0, 0}; // Stock de cada producto

// Declaración de funciones
int file_parser(char *filename); // Función que parsea el archivo filaname y rellena orders con  
void * producer(void *arg); // Función que produce las órdenes. Se usa para los threads productores.
void * consumer(); // Función que consume las órdenes. Se usa para los threads consumidores.
void print_queue(queue *q); // Prints queue, used for debugging


int main (int argc, const char * argv[])
{

    // Control de paso de parámetros
    if (argc != 5 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[4]) <= 0){
        perror("Usage: ./store manager <file name><num producers><num consumers><buff size>\n");
        return -1;
    }

    // Asignación del paso de parámetros a variables
    char filename[FILE_NAME_MAX_SIZE];
    strcpy(filename, argv[1]);
    int n_producers = atoi(argv[2]);
    int n_consumers = atoi(argv[3]);
    int queue_size = atoi(argv[4]);


    // Parseamos el fichero, rellenando el array orders con su información y obteniendo el número de órdenes (num_orders)
    // El parseo del fichero lo realiza la función file_parser
    num_orders = file_parser(filename);
    if (num_orders == -1){
        perror("Error parsing file\n");
        return -1;
    }

    // Inicializamos la cola de operaciones pasándole el tamaño del buffer como tamaño
    or_queue = queue_init((int)queue_size);


    // Inicializar mutex y variables condicionales
    pthread_mutex_init(&mutex, NULL);

    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // Repartir las operaciones de forma equitativa entre los productores
    // A cada productor le corresponde num_orders / n_producers operaciones.
    pthread_t *th_producers = (pthread_t *)malloc(sizeof(pthread_t) * n_producers);
    if (th_producers == NULL){
        perror("Error allocating memory for producers\n");
        return -1;
    }
    // range r; // Estructura que contiene el rango de operaciones que le corresponde a cada productor. Se pasa como argumento a cada thread productor.
    range *ranges = (range *)malloc(sizeof(range) * n_producers);
    if (ranges == NULL){
        perror("Error allocating memory for ranges\n");
        return -1;
    }
    int orders_remaining = num_orders; // orders_remaining sirve para almancenar las ordenen que quedan por repartir entre los productores.
    int producers_remaining = n_producers; // número de productores aún libres
    int last_order_produced = -1; // Última orden producida. Se usa para calcular el rango de operaciones que le corresponde a cada productor.
    int orders_to_produce_this_iteration = 0; // Número de órdenes que le corresponde a cada productor en cada iteración

    // Algoritmo que calcula el número de operaciones que le corresponde a cada productor y crea los threads productores
    for (int i = 0; i < n_producers; i++){
        orders_to_produce_this_iteration = orders_remaining / producers_remaining; // Número de órdenes que le corresponde a este productor
        ranges[i].start = last_order_produced + 1; // Órden por la que empezar a procesar
        ranges[i].end = last_order_produced + orders_to_produce_this_iteration; // Última orden que va a ser procesada
        
        last_order_produced = ranges[i].end; // actualizamos last_order_produced para poder calcular el rango de operaciones del siguiente productor
        orders_remaining -= orders_to_produce_this_iteration; // Se resta al número de ordenes por procesar las órdenex que van a ser procesadas
        producers_remaining--; // Se resta al número de productores libres el productor que va a ser invocado
        
        pthread_create(&th_producers[i], NULL, producer, (void *)&ranges[i]); // Se crea el thread
    }
    
    
    // Crear los threads consumidores
    pthread_t *th_consumers = (pthread_t *)malloc(sizeof(pthread_t) * n_consumers);
    if (th_consumers == NULL){
        perror("Error allocating memory for consumers\n");
        return -1;
    }

    for (int i = 0; i < n_consumers; i++){
        pthread_create(&th_consumers[i], NULL, consumer, NULL);
    }

    // Esperar a que todos los threads productores terminen
    for (int i = 0; i < n_producers; i++){
        pthread_join(th_producers[i], NULL);
    }

    // Esperar a que todos los threads consumidores terminen
    for (int i = 0; i < n_consumers; i++){
        pthread_join(th_consumers[i], NULL);
    }

    // Output
    printf("Total: %d euros\n", profits);
    printf("Stock:\n");
    printf("  Product 1: %d\n", product_stock[0]);
    printf("  Product 2: %d\n", product_stock[1]);
    printf("  Product 3: %d\n", product_stock[2]);
    printf("  Product 4: %d\n", product_stock[3]);
    printf("  Product 5: %d\n", product_stock[4]);

    // Destruir mutex y variables condicionales
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    // Liberar memoria
    queue_destroy(or_queue); // Se libera el espacio de la cola de operaciones
    free(ranges); // Se libera el espacio reservado para la estructura range
    free(th_producers); // Se libera el espacio reservado para los threads productores
    free(th_consumers); // Se libera el espacio reservado para los threads consumidores
    free(orders); // Se libera el espacio reservado para orders
    return 0;
}

void * producer(void *arg){
    /*
    Función productor, se pasa a un hilo y se encarga de procesar las operaciones en el rango r.start, r.end, ambos inclusive
    */

    range r;
    r = *(range *)arg; // Se hace un cast para obtener el rango de operaciones que le corresponde a este productor

    for (int i = r.start; i <= r.end; i++){ // Se itera sobre las operaciones que le corresponden al thread
        pthread_mutex_lock(&mutex); // Se bloquea el mutex para acceder a la cola de operaciones

        // Espera activa hasta que haya hueco en la cola de operaciones
        while (queue_full(or_queue)){
            pthread_cond_wait(&not_full, &mutex);
        }

        // Una vez hay hueco en la cola, se añade la operación a la cola
        queue_put(or_queue, &orders[i]);

        
        pthread_cond_signal(&not_empty); // Se desbloquea a los consumidores para que puedan consumir la operación
        pthread_mutex_unlock(&mutex); // Se libera el mutex

    }
    
    pthread_exit(NULL); // Se elimina el hilo productor
}

void * consumer(){
    /*
    Función consumidor, se pasa a un hilo y se encarga de consumir las operaciones de la cola de operaciones.
    */

    pthread_mutex_lock(&mutex); // Se bloquea el mutex al acceder a la variable orders_consumed

    for(;orders_consumed < num_orders;orders_consumed++){
        
        // En el caso de que la cola esté vacía puede ser por:
        // a) Los productores aún tienen que producir. En este caso simplemente espera a que llegue la variable condicional not_empty
        // b) Ya se han producido y consumido todas las operaciones. En este caso, ya no es necesario consumir más y se termina el consumidor
        while (queue_empty(or_queue)){
            pthread_cond_wait(&not_empty, &mutex);

            // En caso de que otros conumidores ya hayan terminado de consumir las operaciones que quedaban, terminar el consumidor
            if (orders_consumed >= num_orders){
                pthread_cond_signal(&not_empty); // Se desbloquea a los otros consumidores para que puedan terminar
                pthread_mutex_unlock(&mutex); // Se libera el mutex
                pthread_exit(NULL); // Se termina el hilo consumidor
            } 
        }

        
        // Se procede a consumir una orden
        struct element *order = queue_get(or_queue); // Se obtiene la orden a consumir de la cola

        pthread_cond_signal(&not_full); // Se desbloquea a los productores para que puedan seguir produciendo, pues hay hueco en la cola

        // Se procesa la orde, actualizando el stock (product_stock) y los beneficios (profits)
        if (order->op == 0){ // Compra
            profits -= order->units * price_table[order->product_id - 1][BUYING_COST]; // El beneficio disminuye al ser una compra
            product_stock[order->product_id - 1] += order->units; // El stock aumenta
        }
        else { // Venta
            profits += order->units * price_table[order->product_id - 1][SELLING_PRICE]; // El beneficio aumenta al ser una venta
            product_stock[order->product_id - 1] -= order->units; // El stock disminuye
        }
        pthread_mutex_unlock(&mutex); // Una vez ya actualizado orders_consumed, la cola, profits y el stock, se desbloquea el mutex

        free(order); // Se libera la memoria de la orden consumida (queue.c hace malloc)

        pthread_mutex_lock(&mutex); // Se bloquea el mutex para acceder en la siguiente iteración a orders_consumed sabiendo que está actualizado
    }

    pthread_cond_signal(&not_empty); // Se desbloquea a los otros consumidores para que puedan terminar
    pthread_mutex_unlock(&mutex); // Se libera el mutex al ya no tener que acceder ni al la cola ni a la variable orders_consumed

    pthread_exit(NULL); // Se termina el hilo consumidor
}


int file_parser(char *filename){
    /*
    file_parser lee fila por fila el fichero filename
    rellenando el array global orders con orders, 1 por cada fila
    Devuelve el número de ordenes especificado en la primera fila.
    */

    // Abrir el fichero de forma formateada
    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        perror("Error opening file. It may not exist.\n");
        return -1;
    }

    // Leer la primera línea del fichero, para obtener el número de operaciones 
    char line[50]; // Ver si el límite de las líneas es 50
    fgets(line, sizeof(line), fd); // Leer la primera línea e introducirla en el buffer line

    // Obtener el número de lineas haciendo atoi a la primera línea
    int num_orders = atoi(line);
    if (num_orders <= 0){
        perror("Number of operations invalid. Check the first line of file.\n");
        return -1;
    }

    // Reservar espacio para el array de orders
    orders = (struct element *)malloc(sizeof(*orders) * num_orders);
    if (orders == NULL){
        perror("Error allocating memory for orders\n");
        return -1;
    }

    char *product_id = NULL;
    char *op = NULL;
    char *units = NULL;
    int i = 0;

    while (i < num_orders && fgets(line, sizeof(line), fd)){

        if ((product_id = strtok(line, " ")) == NULL){
            perror("Product ID invalid. Check the format of the file.\n");
            return -1;
        }

        if ((op = strtok(NULL, " ")) == NULL){
            perror("Operation type invalid. Check the format of the file.\n");
            return -1;
        }

        if ((units = strtok(NULL, " ")) == NULL){
            perror("Quantity invalid. Check the format of the file.\n");
            return -1;
        }

        // Comprobamos que no haya más elementos en la fila actual
        // Si hubiese más elementos, strtok no devolvería NULL
        if (strtok(NULL, " ")  != NULL){
            perror("File format invalid. Too many arguments in a single row.\n");
            return -1;
        }

        // Comprueba que el product_id obtenido es un número
        if ((orders[i].product_id = atoi(product_id)) <= 0){
            perror("Product ID invalid. Check the format of the file.\n");
            return -1;
        }

        if ((orders[i].units = atoi(units)) <= 0){
            perror("Quantity invalid. Check the format of the file.\n");
            return -1;
        }

        if (strcmp(op, "PURCHASE") == 0) orders[i].op = 0;
        else if (strcmp(op, "SALE") == 0 ) orders[i].op = 1;
        else {
            perror("Operation type invalid. Check the format of the file.\n");
            return -1;
        }
        
        i++;
    }

    if (num_orders > i){
        perror("Error: There are less operations than the number of them specified in the first line of file.\n");
        return -1;
    }

    if(fclose(fd)){
        perror("Error closing file\n");
        return -1;
    }
    return num_orders;
}

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
