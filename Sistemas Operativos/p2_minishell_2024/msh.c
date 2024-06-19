//P2-SSOO-23/24

//  MSH main file
// Write your msh source code here

//#include "parser.h"
#include <stddef.h>			/* NULL */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMANDS 8


// files in case of redirection
char filev[3][64];

//to store the execvp second parameter
char *argv_execvp[8];

void siginthandler(int param)
{
	printf("****  Exiting MSH **** \n");
	//signal(SIGINT, siginthandler);
	exit(0);
}

/* myhistory */

/* myhistory */

struct command
{
  // Store the number of commands in argvv
  int num_commands;
  // Store the number of arguments of each command
  int *args;
  // Store the commands
  char ***argvv;
  // Store the I/O redirection
  char filev[3][64];
  // Store if the command is executed in background or foreground
  int in_background;
};

int history_size = 20;
struct command * history;
int head = 0;
int tail = 0;
int n_elem = 0;

void free_command(struct command *cmd)
{
    if((*cmd).argvv != NULL)
    {
        char **argv;
        for (; (*cmd).argvv && *(*cmd).argvv; (*cmd).argvv++)
        {
            for (argv = *(*cmd).argvv; argv && *argv; argv++)
            {
                if(*argv){
                    free(*argv);
                    *argv = NULL;
                }
            }
        }
    }
    free((*cmd).args);
}

void store_command(char ***argvv, char filev[3][64], int in_background, struct command* cmd)
{
    int num_commands = 0;
    while(argvv[num_commands] != NULL){
        num_commands++;
    }

    for(int f=0;f < 3; f++)
    {
        if(strcmp(filev[f], "0") != 0)
        {
            strcpy((*cmd).filev[f], filev[f]);
        }
        else{
            strcpy((*cmd).filev[f], "0");
        }
    }

    (*cmd).in_background = in_background;
    (*cmd).num_commands = num_commands-1;
    (*cmd).argvv = (char ***) calloc((num_commands) ,sizeof(char **));
    (*cmd).args = (int*) calloc(num_commands , sizeof(int));

    for( int i = 0; i < num_commands; i++)
    {
        int args= 0;
        while( argvv[i][args] != NULL ){
            args++;
        }
        (*cmd).args[i] = args;
        (*cmd).argvv[i] = (char **) calloc((args+1) ,sizeof(char *));
        int j;
        for (j=0; j<args; j++)
        {
            (*cmd).argvv[i][j] = (char *)calloc(strlen(argvv[i][j]),sizeof(char));
            strcpy((*cmd).argvv[i][j], argvv[i][j] );
        }
    }
}


/**
 * Get the command with its parameters for execvp
 * Execute this instruction before run an execvp to obtain the complete command
 * @param argvv
 * @param num_command
 * @return
 */
void getCompleteCommand(char*** argvv, int num_command) {
	//reset first
	for(int j = 0; j < 8; j++)
		argv_execvp[j] = NULL;

	int i = 0;
	for ( i = 0; argvv[num_command][i] != NULL; i++)
		argv_execvp[i] = argvv[num_command][i];
}

int isvalidint(char *number){
    /* Comprueba si los operandos que se usan en mycalc son valores numéricos válidos (números positivos o negativos) */
    for (int i = 0; i < strlen(number); i++) {
        
        if (i == 0 && isdigit(number[i]) == 0 && number[i] != '-') {
            /* Comprueba si el primer dígito es o número o un guion que sirve como signo negativo */
            return 1;
        }
        else if(i != 0 && isdigit(number[i]) == 0) {
            /* Comprueba que el resto de dígitos son números */
            return 1;
        }
    }
    return 0;
}

int mycalc(char ***argvv){

    /* Obtener el número de argumentos en num_args para luego comprobar que coincidan con el uso esperado del comando*/
    int num_args = 0;
    while (argvv[0][num_args] != NULL){
        num_args++;
    }

    /* Comprobar que la sintaxis es correcta: mycalc <operando_1> <add/mul/div> <operando_2> */
    if ((num_args != 4)
    || ((strcmp(argvv[0][2], "add") != 0 && strcmp(argvv[0][2], "mul") != 0 && strcmp(argvv[0][2], "div") != 0))
    || (isvalidint(argvv[0][1]) != 0 || isvalidint(argvv[0][3]) != 0)){
        
        printf("[ERROR] La estructura del comando es mycalc <operando_1> <add/mul/div> <operando_2>\n");
        /* No se hace exit en caso de error para que siga ejecutándose el proceso padre */
    }
    else {
        /* Realizar la operación elegida y mostrar por pantalla los resultados */
        if (strcmp(argvv[0][2], "add") == 0){
            /* Suma de dos operandos. En este caso es necesario actualizar la variable de entorno "Acc" */

            /* Actualizar variable de entorno Acc (creada antes de bucle while (1))*/
            int acc = atoi(getenv("Acc")); /* Obtiene la variable de entorno */
            acc += atoi(argvv[0][1]) + atoi(argvv[0][3]); /* Se calcula el nuevo valor de la variable sumándole ambos sumandos */

            char acc_str[10]; /* Se crea un string para hacer el cast de int a string*/
            sprintf(acc_str, "%d", acc); /* Se hace el cast de int a string, guardándose el valor en acc_str*/
            setenv("Acc", acc_str, 1); /* Se guarda la variable de entrono actualizada */

            /* Se imprime por la salida del error el resultado de la operación add */
            fprintf(stderr, "[OK] %s + %s = %d; Acc %d\n", argvv[0][1], argvv[0][3], atoi(argvv[0][1]) + atoi(argvv[0][3]), acc);
        }
        else if (strcmp(argvv[0][2], "mul") == 0){
            /* Multiplicación de dos operandos */

            /* Se imprime por la salida del error el resultado de la operación mul */
            fprintf(stderr, "[OK] %s * %s = %d\n", argvv[0][1], argvv[0][3], atoi(argvv[0][1]) * atoi(argvv[0][3]));
        }
        else if (strcmp(argvv[0][2], "div") == 0){
            /* División de dos operandos */

            /* Se imprime por la salida del error el resultado de la operación div */
            fprintf(stderr, "[OK] %s / %s = %d; Resto %d\n", argvv[0][1], argvv[0][3], atoi(argvv[0][1]) / atoi(argvv[0][3]), atoi(argvv[0][1]) % atoi(argvv[0][3]));
        }
    }
}

void myhistory_print_command(struct command cmd){
    /* Imprime una secuencia de comandos */

    /* Iteración para imprimir los comandos */
    int i = 0, j = 0; /* i representa el comando, j los argumentos dentro de cada comando*/
    for ( i = 0; i < cmd.num_commands; i++){

        /* Itera sobre cada argumento dentro de un comando imprimiéndolo */
        for ( j = 0; j < cmd.args[i]; j++){
            fprintf(stderr, "%s ", cmd.argvv[i][j]);
        }

        /* En caso de haberse terminado de imprimir el último comando, no se imprime la separación entre comandos */
        if (i + 1 < cmd.num_commands){
            fprintf(stderr, "| ");
        }
        j = 0; /* Tras imprimir un comando entero, el número del argumento actual (j) se reinicia */
    }

    /* Comprobamos si hay redirección de ficheros. Imprimimos el nombre de los ficheros. */
    if (strcmp(cmd.filev[0], "0")){ 
        /* Fichero de entrada */
        fprintf(stderr, "< %s ", cmd.filev[0]);
    }
    if (strcmp(cmd.filev[1], "0")){ 
        /* Fichero de salida */
        fprintf(stderr, "> %s ", cmd.filev[1]);
    }
    if (strcmp(cmd.filev[2], "0")){ 
        /* Fichero de errores */
        fprintf(stderr, "!> %s ", cmd.filev[2]);
    }

    /* Se imprime "&" en caso de haberse ejecutado en modo background* */
    if (cmd.in_background == 1){
        fprintf(stderr, "&");
    }
    fprintf(stderr, "\n");
}

void myhistory_enqueue(char ***argvv, char filev[3][64], int in_background){
    /* Almacenar el comando actual en history (enqueue)*/
    /* history, history_size, n_elem son variables globales, por lo tanto no se pasan como argumentos*/

    /* n_elem son el número de elementos actualmente en la lista */
    /* history_size es el tamaño máximo de la lista*/

    /* Compruebo si la cola ya está llena, en ese caso: */
    int i = 0;
    if (n_elem >= history_size){
        /* Desplazamos todos los comandos una posición hacia abajo, dejando un hueco en la última posición (19) */
        for (int i = 0; i < n_elem - 1; i++){
            history[i] = history[i + 1];
        }
    }
    else{
        /* Si la cola no está llena, aumentar el número de elementos*/
        n_elem ++;
    }

    /* Almacenar el nuevo comando en la última posición disponible */
    store_command(argvv, filev, in_background, &history[n_elem - 1]);
}


int myhistory(char ***argvv, struct command *history){

    /* Obtener el número de argumentos para luego comprobar la sintaxis */
    int num_args = 0;
    while(argvv[0][num_args] != NULL){
        num_args++;
    }

    if (num_args > 2){
        /* En caso de que el número de argumentos sea mayor que dos
        es decir, que no sea el caso de imprimir las últimas 20 secuencias de mandatos o ejecutar un cierto mandato,
        devolver un error */
        printf("Usage: myhistory [n]\n");
        /* No se hace exit en caso de error para que siga ejecutándose el proceso padre */

    }
    else if (num_args == 1){
        /* Caso en el que el usuario no haya introducido ningún comando previamente */
        if (n_elem == 0){
            printf("History is empty\n");
        }
        /* Imprimir los últimos 20 comandos introducidos */
        /* Se itera sobre history, un vector de struct command */
        for (int i = 0; i < n_elem; i++){
            fprintf(stderr, "%d ", i);
            myhistory_print_command(history[i]); /* Se imprime el commando usando la función auxiliar myhistory_print_command*/
        }
    }
    else if (num_args == 2){
        /* Ejecutar el comando especificado por argvv[0][1] */

        /* Se comprueba que el número del comando introducido es menor que el número de comandos que hay en history y que tampoco es menor que 0*/
        if (atoi(argvv[0][1]) < 0 || atoi(argvv[0][1]) >= n_elem){
            printf("ERROR: Comando no encontrado\n");
            /* No se hace exit en caso de error para que siga ejecutándose el proceso padre */
        }
        else{
            /* En caso de que el número del mandato sea correcto, se ejecuta ese comando usando la función execute_command_sequence */
            int i = atoi(argvv[0][1]);
            fprintf(stderr, "Ejecutando el comando %d\n", i);
            execute_command_sequence(history[i].argvv, history[i].filev, history[i].in_background, history[i].num_commands);
        }
    }

}

int execute_command_sequence(char ***argvv, char filev[3][64], int in_background, int command_counter){

    /* Crea un vector de pipes para comunicar los procesos entre sí.
       Utilizamos pipes para comunicar el primer proceso con el siguiente, y así hasta el final de la secuencia.
       Los comandos intermedios leen del pipe anterior el resultado, y lo escriben en el siguiente para comunicarselo al siguiente proceso. */
    int p1[command_counter - 1][2]; 
    
    for (int i = 0; i < command_counter; i++){
        
        /* Control de errores para los pipes */
        if(pipe(p1[i]) < 0){
            perror("Error: pipe failed.\n");
            exit(-1);
        };

        int pid = fork();

        switch(pid){
            case -1:
                /* Control de errores para el fork */
                perror("Error: Fork failed\n");
                exit(-1);

            case 0:
                /* Proceso hijo */
                /* Separamos casos en función del comando a ejecutar*/
                if (i == 0){
                    /* Primer comando */

                    /* Solo usamos los pipes si es una secuencia de comandos */
                    if(command_counter > 1){
                        /* Configurar pipe de escritura */
                        close(STDOUT_FILENO); // Cierro std_out
                        close(p1[i][0]); // Cierro pipe de lectura
                        dup(p1[i][1]); // Duplico pipe de escritura
                        close(p1[i][1]); // Cierro pipe de escritura
                    }
                    else{
                        /* Caso de un único comando */
                        /* Redirección de salida a filev[1] (si hiciera falta)*/
                        if (strcmp(filev[1], "0") != 0){
                        
                            int fd_out = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0777); // Abrimos el archivo filev[1]
                            if (fd_out == -1){
                                perror("Error: Open failed\n");
                                exit(-1);
                            }
                            /* Cerramos la salida estándar y la sustituimos por filev[1]*/
                            close(STDOUT_FILENO);
                            dup(fd_out);
                            close(fd_out);
                        }
                    }

                    /* Reedirección de entrada del primer comando a filev[0] */
                    if (strcmp(filev[0], "0") != 0){

                        int fd_in = open(filev[0], O_RDONLY); // Abrimos el archivo filev[0]
                        if (fd_in == -1){
                            perror("Error: Open failed\n");
                            exit(-1);
                        }
                        /* Cerramos la entrada estándar y lo sustituimos por filev[0] */
                        close(STDIN_FILENO);
                        dup(fd_in);
                        close(fd_in);
                    }
                }
                else if (i == command_counter - 1){
                    /* Último comando */

                    /* Config pipe (previo) de lectura */
                    close(STDIN_FILENO); // Cierro std_in
                    dup(p1[i -1][0]); // Duplico pipe de lectura
                    close(p1[i-1][1]); // Cierro pipe de escritura
                    close(p1[i-1][0]); // Cierro pipe de lectura        

                    /* Redirección de salida a filev[1] */
                    if (strcmp(filev[1], "0") != 0){
                    
                        int fd_out = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0777); // Abrimos el archivo filev[1]
                        if (fd_out == -1){
                            perror("Error: Open failed\n");
                            exit(-1);
                        }
                        /* Cerramos la salida estándar y la sustituimos por filev[1]*/
                        close(STDOUT_FILENO);
                        dup(fd_out);
                        close(fd_out);
                    }
    
                }
                else{
                    /* Comandos intermedios (leen y escriben) */

                    /* Config pipe (previo) de lectura.
                       Lee el dato que devuelve el comando anterior.*/
                    close(STDIN_FILENO);
                    dup(p1[i-1][0]);
                    close(p1[i-1][1]);
                    close(p1[i-1][0]);

                    /* Config pipe (actual) de escritura.
                       Escribe el dato en el pipe (actual) para mandarselo al siguiente comando.*/
                    close(STDOUT_FILENO);
                    dup(p1[i][1]);
                    close(p1[i][0]);
                    close(p1[i][1]);

                }
            
                /* Redirección de errores a filev[2] */
                if (strcmp(filev[2], "0") != 0){
                    int fd_err = open(filev[2], O_WRONLY | O_CREAT | O_TRUNC, 0777); // Abrimos el archivo filev[2]
                    /* Control de errores */
                    if (fd_err == -1){ 
                        perror("Error: Open failed\n");
                        exit(-1);
                    }
                    /* Cerramos la salida estándar de error y lo sustituimos por filev[2] */
                    close(STDERR_FILENO);
                    dup(fd_err);
                    close(fd_err);
                }
                
                /* Ejecutamos los comandos */
                execvp(argvv[i][0], &argvv[i][0]);
                perror("Error: Execvp failed\n");
                exit(-1);
                break;

            default:
                /* Proceso padre (MSH)*/

                /* Cierro los pipes ya utilizados */
                if(i > 0){
                    // A partir de la segunda iteración
                    close(p1[i-1][0]);
                    close(p1[i-1][1]);
                }
                /* Check del tipo de ejecución de comandos */
                if (in_background == 0){
                    /* Ejecución en foreground */
                    if (waitpid(pid, NULL, 0) == -1){
                        perror("Error: Wait failed\n");
                        exit(-1);
                    }
                }
                else {
                    /* Ejecución en background */
                    printf("[%d]\n", pid);
                }
                break;
        }
    } 
}

/**
 * Main sheell  Loop  
 */
int main(int argc, char* argv[])
{
	/**** Do not delete this code.****/
	int end = 0; 
	int executed_cmd_lines = -1;
	char *cmd_line = NULL;
	char *cmd_lines[10];

	if (!isatty(STDIN_FILENO)) {
		cmd_line = (char*)malloc(100);
		while (scanf(" %[^\n]", cmd_line) != EOF){
			if(strlen(cmd_line) <= 0) return 0;
			cmd_lines[end] = (char*)malloc(strlen(cmd_line)+1);
			strcpy(cmd_lines[end], cmd_line);
			end++;
			fflush (stdin);
			fflush(stdout);
		}
	}

	/*********************************/

	char ***argvv = NULL;
	int num_commands;

	history = (struct command*) malloc(history_size *sizeof(struct command)); /* Se usa como una cola */
	int run_history = 0;
    
    setenv("Acc", "0", 1); /* Variable de entorno para guardar las suma de sumas de mycalc*/

	while (1) 
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		if (run_history)
    {
        run_history=0;
    }>
    else{
        // Prompt 
        write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

        // Get command
        //********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
        executed_cmd_lines++;
        if( end != 0 && executed_cmd_lines < end) {
            command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
        }
        else if( end != 0 && executed_cmd_lines == end)
            return 0;
        else
            command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
    }
		//************************************************************************************************


		/************************ STUDENTS CODE ********************************/
	   if (command_counter > 0) {
            /* Comprobar que la secuencia introducida no excede el número máximo de comandos */
			if (command_counter > MAX_COMMANDS){
				printf("Error: Maximum number of commands is %d \n", MAX_COMMANDS);
			}
			else {

                /* Comprobar si el comando introducido es mycalc */
                if (strcmp(argvv[0][0], "mycalc") == 0){

                    mycalc(argvv); /* Ejecutar comando mycalc */
                }
                /* Comprobar si el comando introducido es myhistory*/
                else if (strcmp(argvv[0][0], "myhistory") == 0){

                    myhistory(argvv, history); /* Ejecutar comando myhistory */
                }
                else {
                    /* Si el comando introducido no es ni mycalc ni myhistory,
                    Ejecutar la secuencia recibida como si de una terminal de unix
                    se tratase*/ 
                    
                    myhistory_enqueue(argvv, filev, in_background); /* Almacena el comando actual en la cola de myhistory */
                    
                    execute_command_sequence(argvv, filev, in_background, command_counter); /* Ejecuta la secuencia de comandos introducida */

                }
			}
		}
	}
	return 0;
}
