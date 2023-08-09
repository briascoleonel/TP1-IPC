#include "myqueue.h"
#include <stdlib.h>

node_t* cabeza = NULL;
node_t* cola = NULL;


void enqueue(int *client_socket)
{
    node_t *newnode = malloc(sizeof(node_t));
    newnode->client_socket = client_socket;
    newnode->next = NULL;
    if(cola == NULL)
    {
        cabeza = newnode;
    }
    else
    {
        cola->next = newnode;    
    }
    cola = newnode;
}

//Devuelve NULL si la cola esta vacia, devuelve el puntero a client_socket si hay uno para agarrar

int* dequeue()
{
    if(cabeza == NULL)
    {
        return NULL;
    }
    else
    {
        int *result = cabeza->client_socket;
        node_t *temp = cabeza;
        if(cabeza == NULL)
        {
            cola = NULL;
        }
        free(temp);
        return result;
    }
}