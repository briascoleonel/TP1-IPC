#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <pthread.h>                //Para funciones thread
#include "myqueue.h"                

#define SERVERPORT  8989
#define BUFSIZE     4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100          //Cantidad de conexiones que puede esperar
#define THREAD_POOL_SIZE 5        //Cantidad de threads que vamos a correr

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;            //Permite a los threads esperar por alguna condicion con wait y signal

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void * handle_connection(void* p_client_socket);
int check(int exp, const char *msg);
void * thread_function(void *arg);

int main(int argc, char **argv)
{
    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;

    //Creamos 20 threads para futuras conexiones
    for (int i=0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);
    }

    //Utilizando la funcion check: chequea errores tipicos
    //Creacion de socket
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Fallo al crear el socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVERPORT); //8989

    //Bind y esuchamos en el puerto
    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Fallo el bind");
    check(listen(server_socket, SERVER_BACKLOG), "Fallo el listen");


    //Loop infinito que acepta conexiones
    while(true)
    {
        printf("Esperando por conexiones...\n");
        addr_size = sizeof(SA_IN);
        check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "Fallo el accept");
        printf("Conectado!\n");

        //Pasa cada conexion al handler

        //Pone la conexion en un lugar que el thread pueda encontrarla
        //Usamos una cola

        //handle_connection(client_socket);
        //pthread_t t;
        int *pclient =  malloc(sizeof(int));
        *pclient = client_socket;
        pthread_mutex_lock(&mutex);
        enqueue(pclient);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
        //pthread_create(&t, NULL, handle_connection, pclient);     //CON THREADS
        //handle_connection(pclient);     //SIN THREADS
    }

    return 0;

}

int check(int exp, const char *msg)
{
    if(exp == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}

void * thread_function(void *arg)
{
    while(true)
    {
        int *pclient;  
        pthread_mutex_lock(&mutex);    
        if((pclient = dequeue()) == NULL)
        {
            pthread_cond_wait(&condition_var, &mutex);      //Van a esperar solo si no tienen que esperar la cola
            pclient = dequeue();
        }
        pthread_mutex_unlock(&mutex);    

        if(pclient != NULL)     //Hay una conexion
        {
            handle_connection(pclient);
        }
    }
}

//El cliente envia el nombre del archivo y el server lee y devuelve el archivo solicitado(simil web server)
//No es seguro ya que puede leer cualquier archivo del disco duro
void * handle_connection(void* p_client_socket)
{
    int client_socket = *((int*)p_client_socket);
    free(p_client_socket);
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //Lee el mensaje del cliente
    while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1)) > 0)
    {
        msgsize += bytes_read;
        //Con limite en el BUFSIZE para tamaño o si recibe un salto de linea
        if(msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n')
        break;
    }
    //Chequeamos errrores
    check(bytes_read, "Error de recepecion");
    buffer[msgsize-1] = 0;      //Terminamos el buffer

    //Imprime REQUEST para ver que esta ocurriendo
    printf("REQUEST: %s\n", buffer);
    fflush(stdout);

    //Usa realpath para comprobar que el actual es real
    if(realpath(buffer, actualpath) == NULL)
    {
        printf("Error(bad path): %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    //Abre el archivo solicitado
    FILE *fp = fopen(actualpath, "r");
    if(fp == NULL)
    {
        printf("Error(abriendo): %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    sleep(1);       //No hace nada(version lenta)

    //Leemos el documento
    while ((bytes_read = fread(buffer,1,BUFSIZE, fp)) > 0)
    {
        printf("Enviando %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);       //Envia el documento
    }
    close(client_socket);           //Cerramos el socket
    fclose(fp);                     //Cerramos el archivo
    printf("Cerrando conexion\n");  //Conexion terminada
    return NULL;

}