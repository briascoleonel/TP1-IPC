#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <sys/select.h>


#define SERVERPORT      8989
#define BUFSIZE         4096
#define SOCKETERROR     (-1)
#define SERVER_BACKLOG  100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

//Prototipos
void * handle_connection(int);
int check(int exp, const char *msg);
int accept_new_connection(int server_socket);
int setup_server(short port, int backlog);

int main(int argc, char **argv)
{
    int server_socket = setup_server(SERVERPORT, SERVER_BACKLOG);

    fd_set current_sockets, ready_sockets;

    //inicializa el set
    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);

    while(true)
    {
        //Select es destructiva
        ready_sockets = current_sockets;

        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error de select");
            exit(EXIT_FAILURE);
        }

        for(int i=0; i < FD_SETSIZE; i++)
        {
            if(FD_ISSET(i, &ready_sockets))
            {
                if(i == server_socket)
                {
                    //Nueva conexion
                    int client_socket = accept_new_connection(server_socket);
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    handle_connection(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

int setup_server(short port, int backlog)
{
    int server_socket, client_socket, addr_size;
    SA_IN server_addr;

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Fallo al crear socket");

    server_addr.sin_family =        AF_INET;
    server_addr.sin_addr.s_addr =   INADDR_ANY;
    server_addr.sin_port=           htons(port);

    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Fallo el bind!");
    check(listen(server_socket, backlog), "Fallo el listen");
    
    return server_socket;
}

int accept_new_connection(int server_socket)
{
    int addr_size = sizeof(SA_IN);
    int client_socket;
    SA_IN client_addr;
    check(client_socket = accept(server_socket, (SA*)&client_addr,(socklen_t*)&addr_size), "Fallo el accept");

    return client_socket;
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

void * handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if(msgsize > bytes_read || buffer[msgsize-1] == '\n')
        break;
    }

    check(bytes_read, "Recieve error");
    buffer[msgsize-1] = 0;

    printf("REQUEST: %s\n", buffer);
    fflush(stdout);

    if(realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR(bad path): %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    FILE *fp = fopen(actualpath, "r");
    if(fp == NULL)
    {
        printf("ERROR(open): %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    while((bytes_read = fread(buffer,1,BUFSIZE,fp))>0)
    {
        write(client_socket,buffer,bytes_read);
    }
    close(client_socket);
    fclose(fp);
    printf("Cerrando conexion\n");
    return NULL;
}