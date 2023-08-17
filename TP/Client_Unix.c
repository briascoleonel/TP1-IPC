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
#include <pthread.h>                //Para funciones thread
#include <sys/un.h>

#define SA struct sockaddr
#define MAXLINE 4096


int main(int argc, char *argv)
{
    int sockfd;
    struct sockaddr_un server_addr;

    long unsigned int env_bytes;
    long int esc_val_ret;

    char string[MAXLINE];
    char aux[MAXLINE];
    char fin_de_msg[7] = "\n";

    long unsigned int veces_enviado;
    long unsigned int cont = 0;

    
}