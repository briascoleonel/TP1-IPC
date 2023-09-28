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
#include <sys/time.h>
#include <stdarg.h>
#include <pthread.h>          //Para funciones thread
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
#include "Handler.h"
#include "Contador.h"
#include "Common.h"


int dir_IPv4_valida(char *ipAddr)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET,ipAddr, &(sa.sin_addr));
    return result != 0;
}

int dir_IPv6_valida(char *ipAddr)
{
    struct sockaddr_in6 sa;
    int result = inet_pton(AF_INET6,ipAddr, &(sa.sin6_addr));
    return result != 0;
}

void ocupar_handler(int *Handlers, int i, pthread_mutex_t *lock)
{
    pthread_mutex_lock(lock);
    Handlers[i] = 0;
    pthread_mutex_unlock(lock);
}

void liberar_Handler(int *Handlers, int i, pthread_mutex_t *lock)
{
    pthread_mutex_lock(lock);
    Handlers[i] = 1;
    pthread_mutex_unlock(lock);
}

int filename_valido(char *string) 
{
    //Controla la existencia de los siguientes caracteres: \/:*?"<>|
    if((strchr(string,(int)'/') != NULL) || (strchr(string,(int)':') != NULL) || (strchr(string,(int)'*') != NULL) || (strchr(string,(int)'?') != NULL) || 
    (strchr(string,(int)'<') != NULL) || (strchr(string,(int)'>') != NULL) || (strchr(string,(int)'|') != NULL))
    {
        return 0;
    }
    return 1;
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

void Hndlr(union sigval sigev_value) {
    struct Info_t* info = sigev_value.sival_ptr;
    long unsigned int bytesRec;
    long unsigned int lastByteAmount;
    long unsigned int bandwidth;
    info->destFile = fopen(info->filename, "a"); 
    pthread_mutex_lock(info->lock);
    bytesRec = *(info->TotalBytesRcv);
    bandwidth = bytesRec/info->cont;
    lastByteAmount = *(info->LastBytesRcv);
    *(info->LastBytesRcv) = 0;
    pthread_mutex_unlock(info->lock);
    
    if(bytesRec != 0)
    {
        if(fprintf(info->destFile,"Log: %ld | Total de Bytes recibidos: %ld | Bytes recibidos en este segundo: %ld | Bytes/s: %ld\n",
        info->cont,bytesRec,lastByteAmount,bandwidth) < 0)
        {
            printf("Falla al escribir en archivo\n");
            exit(EXIT_FAILURE);
        }
        info->cont++;   
    }
    fclose(info->destFile);
    atomic_store(&info->status, Expire);
}

void TimerInit(struct Info_t* info) {
    int r;
    struct sigevent sev;
    struct itimerspec its;

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_value.sival_ptr = info;
    sev.sigev_notify_function = &Hndlr;
    sev.sigev_notify_attributes = 0;
    r = timer_create(CLOCK_REALTIME, &sev, &info->timerId);
    if(r)
        abort();

    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;
    //its.it_value.tv_sec = seconds;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    r = timer_settime(info->timerId, 0, &its, NULL);
    if(r)
        abort();
}
