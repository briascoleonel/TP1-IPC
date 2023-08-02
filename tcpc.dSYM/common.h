#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/socket.h> /*Definiciones basicas de sockets*/
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>  /*Para funciones con argumentos var*/
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdarg.h>

//Constante para puerto
#define SERVER_PORT 18000

#define MAXLINE     4096
#define SA struct sockaddr



void err_n_die(const char *fmt, ...);                       //Funcion para errores
char *bin2hex(const unsigned char *input, size_t len);      //convierte bytes a hexa y controla si algun caracter que no se puede imprimir

#endif