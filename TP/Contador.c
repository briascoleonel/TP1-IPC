#include "Common.h"

void* Contador_codigo(void *arg)
{
    struct local_threads_arg_struct *argumentos = arg;
    int aux1,aux2,cont = 0;

    while(1)
    {
        if(!argumentos->thread_salida)
        {
            pthread_mutex_lock(argumentos->lock);
            argumentos->segs = 1;
            pthread_mutex_unlock(argumentos->lock);
            aux1 = argumentos->segs;
            usleep(1000000);
            aux2 = argumentos->segs;
            cont++;
            if(aux1 != aux2)
            {
                cont = 0;
            }

            if(cont == 10)
            {
                pthread_mutex_lock(argumentos->lock);
                argumentos->thread_salida = 1;
                pthread_mutex_unlock(argumentos->lock);
                break;
            }
        }
    }
    return NULL;
}