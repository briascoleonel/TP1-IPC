/*Headers*/
#include <sys/socket.h> /*Definiciones basicas de sockets*/
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>  /*Para funciones var*/
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

/*Preproccessor macros para legibilidad*/
#define SERVER_PORT 80 /*Puerto estandar HTTP*/

#define MAXLINE 4096 /*Tamaño buffer*/
#define SA struct sockaddr /*Para usar menos codigo*/

void err_n_die(const char *fmt, ...); /*Maneja errores*/

int main(int argc, char **argv)
{

    /*Variable locales*/
    int     sockfd,n;
    int     sendbytes;
    struct  sockaddr_in  servaddr;
    char    sendline[MAXLINE];
    char    recvline[MAXLINE];


    /*Usage check*/
    if(argc != 2)
        err_n_die("usage: %s <server address>", argv[0]);
    
    /*Crea socket*/
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("Error al crear el socket");

    /*Zero out the address*/
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERVER_PORT);


    /*Translate address a binary*/
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_n_die("inet_pton error for %s ", argv[1]);


    /*Conectar a los servidores*/
    if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_n_die("Conexion fallida");

    /*Ya estamos conectados, creamos linea a enviar*/
    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
    sendbytes = strlen(sendline);


    /*Escribo la request en el socket*/
    if(write(sockfd, sendline, sendbytes) != sendbytes)
        err_n_die("Error de escritura");

    memset(recvline, 0, MAXLINE);

    while( (n = read(sockfd, recvline, MAXLINE-1)) > 0)
    {
        printf("%s", recvline);
    }
    if (n < 0)
        err_n_die("Error de lectura");

    exit(0);

}



void err_n_die(const char *fmt, ...)
{
    int errno_save;
    va_list     ap;

    errno_save = errno;


    va_start(ap,fmt);
    vfprintf(stdout,fmt,ap);
    fprintf(stdout,"\n");
    fflush(stdout);


    if(errno_save != 0)
    {
        fprintf(stdout, "(errno = %d) : %s\n",errno_save,strerror(errno_save));
        fprintf(stdout,"\n");
        fflush(stdout);

    }

    va_end(ap);


    exit(1);
}

