#include "Common.h"

void* Task(void *arg)
{
    struct local_threads_arg_struct *argumentos = arg;

    char recvline[MAXLINE];
    char aux[MAXLINE];
    char rawmsg[MAXLINE];
    long int bytes_lect;

    while(argumentos->thread_salida == 0 && *(argumentos->salir) == 0)
    {
        while(bytes_lect <= 0)
        {
            if(!argumentos->thread_salida)
            {
                bytes_lect = recv(*(argumentos->socket_conx), recvline, MAXLINE-1, MSG_DONTWAIT);
            }
            if(argumentos->thread_salida)
            {
                if(bytes_lect < 0)
                {
                    printf("Error de lectura en %d\n", argumentos->id);
                }
                if(close(*(argumentos->socket_conx)) < 0)
                {
                    printf("Error al cerrar conexcion\n");
                }
                break;
            }
        }
        if(!argumentos->thread_salida && *(argumentos->salir) == 0)
        {
            while(bytes_lect > 0)
            {
                if(recvline[bytes_lect-1] == '\n')
                {
                    memset(aux,0,MAXLINE);
                    memset(rawmsg,0,MAXLINE);
                    strcpy(aux,recvline);
                    int i = 0;
                    while(aux[i] != '\n')
                    {
                        rawmsg[i] = aux[i];
                        i++;
                    }
                    bytes_lect = 0;
                    break;
                }
            }
        }
    }
    liberar_Handler(argumentos->Handlers,argumentos->id, argumentos->lock);
    return NULL;
}