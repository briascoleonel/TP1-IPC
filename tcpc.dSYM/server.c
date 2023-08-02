#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <linux/limits.h>

#define SERVERPORT  8989
#define BUFSIZE     4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void handle_connection(int client_socket);
int check(int exp, const char *msg);

int main(int argc, char **argv)
{
    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;


    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Fallo al crear el socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVERPORT);

    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Fallo el bind");
    check(listen(server_socket, SERVER_BACKLOG), "Fallo el listen");

    while(true)
    {
        printf("Esperando por conexiones...\n");
        addr_size = sizeof(SA_IN);
        check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "Fallo el accept");
        printf("Conectado!\n");

        handle_connection(client_socket);
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

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1)) > 0)
    {
        msgsize += bytes_read;
        if(msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n')
        break;
    }
    check(bytes_read, "Error de recepecion");
    buffer[msgsize-1] = 0;

    printf("REQUEST: %s\n", buffer);
    fflush(stdout);

    if(realpath(buffer, actualpath) == NULL)
    {
        printf("Error(bad path): %s\n", buffer);
        close(client_socket);
        return;
    }
}