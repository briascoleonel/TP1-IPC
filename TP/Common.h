#ifndef COMMON_H    // Comienzo del ifdef guard
#define COMMON_H_H


#include <sys/socket.h>             //Definiciones basicas de sockets
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>                 //Para funciones con argumentos var
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdarg.h>
#include <pthread.h>                //Para funciones thread
#include <ctype.h>
#include <netinet/in.h>             //Para struct de in
#include <net/if.h>                 //Network interfaces
#include <sys/un.h>

#include "Server_Unix.h"
#include "Server_IPv4.h"
#include "Server_IPv6.h"
#include "File_Writing_Thread.h"
#include "Verificar_Argumentos_Server.h"
#include "Config_Socket_Unix.h"
#include "Config_Socket_IPv4.h"
#include "Config_Socket_IPv6.h"

#define SA struct sockaddr
#define MAXLINE 4096

//Struct que vamos a utilizar
struct local_threads_arg_struct {
    int id;
    int *socket_conx;
    int thread_salida;
    int *Handlers;
    pthread_mutex_t *lock;
    pthread_mutex_t *global_lock;
    int *salir;
};

struct UNIX_arg_struct {
    char UNIX_File_Name[MAXLINE];
    char UNIX_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    int max_clientes;
    int *salir;
};

struct local_writer_arg_struct {
    char Write_File_Name[MAXLINE];
    pthread_mutex_t *lock;
    int *salir;
};

struct IPv4_arg_struct {
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    char IPv4_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    int max_clientes;
    int *salir;
};

struct IPv6_arg_struct {
    char IPV6_Server_Address[MAXLINE];
    short unsigned int IPV6_iport;
    char IPV6_Interface[MAXLINE];
    char IPv6_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    int max_clientes;
    int *salir;
};

int filename_valido(char *string); 
int dir_IPv4_valida(char *ipAddr);
int dir_IPv6_valida(char *ipAddr);
void liberar_Handler(int *Handlers, int i, pthread_mutex_t *lock);




#endif 