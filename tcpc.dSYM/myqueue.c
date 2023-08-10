#include "myqueue.h"
#include <stdlib.h>

node_t* head = NULL;
node_t* tail = NULL;


void enqueue(int *client_socket)
{
    node_t *newnode = malloc(sizeof(node_t));
    newnode->client_socket = client_socket;
    newnode->next = NULL;
    if(tail == NULL)
    {
        head = newnode;
    }
    else
    {
        tail->next = newnode;    
    }
    tail = newnode;
}

//Devuelve NULL si la tail esta vacia, devuelve el puntero a client_socket si hay uno para agarrar

int* dequeue()
{
    if(head == NULL)
    {
        return NULL;
    }
    else
    {
        int *result = head->client_socket;
        node_t *temp = head;
        if(head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        return result;
    }
}