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

#include "Server_Unix.h"
#include "Server_IPv4.h"
#include "Server_IPv6.h"
#include "File_Writing_Thread.h"



void ocupar_handler(int *Handlers, int i, pthread_mutex_t *lock)
{
    pthread_mutex_lock(lock);
    Handlers[i] = 0;
    pthread_mutex_unlock(lock);
}

int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers)
{
    for(unsigned long int i = 0; i < maxHandlers; i++)
    {
        if(Handlers[i])
        {
            return (int)i;
        }
    }
    return -1;
}

int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers)
{
    int amount = 0;
    for(unsigned long int i = 0; i < maxHandlers; i++)
    {
        if(Handlers[i])
        {
            amount++;
        }
    }
    return amount;
}
