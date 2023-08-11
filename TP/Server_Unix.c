#include <sys/socket.h> /*Definiciones basicas de sockets*/
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>  /*Para funciones con argumentos var*/
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdarg.h>
#include <pthread.h>                //Para funciones thread
#include <sys/un.h>

#define MAXLINE 4096
#define SA struct sockaddr

void server_conf_socket_Unix(int *sock, struct sockaddr_un *serv_addr, long unsigned int max, char *filename);
void* file_writing_thread(void * arg);
int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers);
int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers);

//Struct que vamos a utilizar

struct UNIX_arg_struct {
    char UNIX_File_Name[MAXLINE];
    char UNIX_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    int maxClientes;
    int *salir;
};

struct local_threads_arg_struct {
    int id;
    int *ConnSocket;
    pthread_mutex_t *lock;
    pthread_mutex_t *global_lock;
};

struct local_writer_arg_struct {
    char Write_File_Name[MAXLINE];
    pthread_mutex_t *lock;
    int *salir;
};

//Funcion principal de servidor tipo UNIX

void *Unix_Server(void *arg)
{
    struct UNIX_arg_struct *argumentos = arg;       //Struct para argumentos
    int listenfd;       //FD para escuchar conexiones
    int *connfd;        //FD para el socket una vez ocurra el accept
    struct sockaddr_un serv_addr;       //Struct para pasar direccion

    connfd = malloc((unsigned long int) argumentos->maxClientes * sizeof(int));

    //Creamos los hilo y mutex para que no haya acceso al mismo tiempo
    pthread_t *task_thread;
    struct local_thread_arg_struct *handler_thread_args;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    //Asignacion de memoria para el thread utilizando cantidad maxima de clientes como referencia
    task_thread = malloc((unsigned long int) argumentos->maxClientes *sizeof(pthread_t));
    handler_thread_args = malloc((unsigned long int) argumentos->maxClientes * sizeof(struct local_threads_arg_struct));

    //Inicializamos handlers para manejar los threads
    long unsigned int cant_handlers_disp = 0;
    int *handlers_disp;         //Bandera para saber cuantos hay disponible
    int sig_handler;            //Utiliza el primer handler que se encuentra

    //Asignacion de memoria
    handlers_disp = malloc((unsigned long int)argumentos->maxClientes *sizeof(int));

    //Hilo para excribir archivo con bytes recibidos
    pthread_t file_writer_thread;
    struct local_writer_arg_struct file_writer_arg;

    strcpy(file_writer_arg.Write_File_Name, argumentos->UNIX_Write_File_Name);
    file_writer_arg.lock = &lock;
    file_writer_arg.salir = &argumentos->salir;

    //Para configurar el socket se utiliza esta funcion
    server_conf_socket_Unix(&listenfd, &serv_addr, (unsigned long int)argumentos->maxClientes, argumentos->UNIX_File_Name);

    //Iniciamos todos los handlers como disponibles
    for(int i=0; i<argumentos->maxClientes; i++)
    {
        handlers_disp[i] = 1;
    }

    //Crea y lanza un nuevo hilo que escribe el archivo local
    //Usa la funcion file_writing_thread que pasa un archivo que se abre de acuerdo a los argumentos que le pasamos
    pthread_create(&file_writer_thread,NULL,file_writing_thread,&file_writer_arg);

    //Esperando conexiones y lanzando hilos
    while(*(argumentos->salir) == 0)
    {
        //En exclusion mutua usando el lock
        pthread_mutex_lock(&lock);
        //Usa la funcion get_cant_hand_disp
        cant_handlers_disp = (long unsigned int) get_cant_hand_disp(handlers_disp,(unsigned long int) argumentos->maxClientes);
        pthread_mutex_unlock(&lock);    //Libero el lock

        //Compruebo que queden handlers disponibles
        //Usa get_prim_hand_disp para comprobar tambien que haya disponibles
        if(cant_handlers_disp == 0 || get_prim_hand_disp(handlers_disp, (unsigned long int)argumentos->maxClientes) < 0)
        {
            while(cant_handlers_disp == 0)
            {
                pthread_mutex_lock(&lock);
                cant_handlers_disp = (long unsigned int)getAvailableHandlersAmount(handlers_disp, (unsigned long int)argumentos->maxClientes);
                pthread_mutex_unlock(&lock);
            }
        }

        //Pasando la prueba de disponibilidad, agarra el primero disponible
        //Tambien en exclusion mutua
        pthread_mutex_lock(&lock);
        sig_handler = get_cant_hand_disp(handlers_disp, (unsigned long int)argumentos->maxClientes);
        pthread_mutex_unlock(&lock);

        fflush(stdout);

        //Espera y acepta una conexion en el handler disponible
        connfd[sig_handler] = accept(listenfd, (SA*) NULL, NULL);

        //Si no hay handler siguiente, salta el error
        if(connfd[sig_handler] == -1)
        {
            printf("Error en accept:\n");
            exit(EXIT_FAILURE);
        }
    }

}

void server_conf_socket_Unix(int *sock, struct sockaddr_un *serv_addr, long unsigned int max, char *filename)
{
    //Creacion de socket
    if((*(sock) = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
    {
        //Imprime error en caso de que asi sea
        printf("Error al crear el socket para el servidor\n");
        exit(EXIT_FAILURE);
    }

    unlink(filename);       //Borra el nombre y el file al que se refiere

    //Matchea el path del archivo del server con el que se quiere conectar
    bzero(serv_addr, sizeof(*serv_addr));
    serv_addr->sun_family = AF_UNIX;
    strcpy(serv_addr->sun_path, filename);

    //Bindea con la direccion especificada
    if((bind(*(sock),(SA*) serv_addr, sizeof(*serv_addr))) < 0)
    {
        //Imprime error en caso de que asi sea
        printf("Error al asociar el socket con la direccion\n");
    }

    //Ponemos a escuchar en el socket
    if((listen(*(sock), (int)max)) < 0)
    {
        printf("Error en la escucha\n");
        exit(EXIT_FAILURE);
    }
}

void* file_writing_thread(void * arg)
{
    struct local_writer_arg_struct *arguments = arg;
    FILE* destFile;
    
    destFile = fopen(arguments->Write_File_Name, "w"); 
    fclose(destFile);

    while(*(arguments->salir) == 0)
    {
    }

    return NULL;
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