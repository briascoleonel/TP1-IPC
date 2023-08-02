#include "common.h"
#include <strings.h>

int main(int argc, char **argv)
{
    //Variables
    int                     listenfd, connfd, n;
    struct  sockaddr_in     servaddr;
    uint8_t                 buff[MAXLINE+1];
    uint8_t                 recvline[MAXLINE+1];

    //Creacion de socket
    if((listenfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
        err_n_die("Error de socket");

    
    //Setea la direccion del server
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    servaddr.sin_port           = htons(SERVER_PORT);           //Puerto 18000

    //Este socket va a escuchar en esta direccion
    if((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
        err_n_die("Error de bind");

    if((listen(listenfd,10)) < 0)
        err_n_die("Error de listen");

    //Loop para aceptar conexiones
    for( ; ; )
    {
        struct sockaddr_in addr;
        socklen_t addr_len;
        char client_address[MAXLINE+1];

        printf("Esperando por una conexion en puerto: %d\n",SERVER_PORT);
        fflush(stdout);
        connfd = accept(listenfd, (SA *) &addr, &addr_len);       //Punteros a addr y addr_len


        inet_ntop(AF_INET, &addr, client_address, MAXLINE);         //Pasa una direccion a fomato presentable
        printf("Conexion del client: %s\n", client_address);        //Imprime la direccion del cliente

        memset(recvline, 0, MAXLINE);

        //Lee desde el socket el mensaje del cliente
        while((n = read(connfd, recvline, MAXLINE-1)) > 0)
        {
            //Imprime el equivalente en binario y texto
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

            //Enter para ver el fin de mensaje
            if(recvline[n-1] == '\n')
            {
                break;
            }

            memset(recvline,0,MAXLINE);
        }
        //Si un byte es negativo no se puede leer
        if(n < 0)
            err_n_die("Error de lectura");


        //Respuesta HTTP (aparece en el browser)
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

        write(connfd, (char*)buff, strlen((char *)buff));
        close(connfd);

    }
}