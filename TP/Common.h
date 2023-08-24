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

#define SA struct sockaddr
#define MAXLINE 4096

//Struct que vamos a utilizar
struct local_threads_arg_struct {
    int id;
    int *ConnSocket;
    int ExitThread;
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

struct INET_arg_struct {
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    char INET_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    int max_clientes;
    int *salir;
};

#endif 