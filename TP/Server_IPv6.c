#include "Common.h"

void server_conf_socket_INET(int *sock, struct sockaddr_in *serv_addr, int iport, long unsigned int max, char *string_addr);
void* file_writing_thread(void * arg);
int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers);
int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers);
void ocupar_handler(int *Handlers, int i, pthread_mutex_t *lock);

int main(void *arg)
{
    struct INET_arg_struct *argumentos = arg;       //Struct para argumentos
    int listenfd;       //FD para escuchar conexiones
    int *connfd;        //FD para el socket una vez ocurra el accept
    struct sockaddr_in serv_addr;       //Struct para pasar direccion

    connfd = malloc((unsigned long int) argumentos->max_clientes * sizeof(int));

    //Creamos los hilo y mutex para que no haya acceso al mismo tiempo
    pthread_t *task_thread;
    struct local_threads_arg_struct *handler_thread_args;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    //Asignacion de memoria para el thread utilizando cantidad maxima de clientes como referencia
    task_thread = malloc((unsigned long int) argumentos->max_clientes *sizeof(pthread_t));
    handler_thread_args = malloc((unsigned long int) argumentos->max_clientes * sizeof(struct local_threads_arg_struct));

    //Inicializamos handlers para manejar los threads
    long unsigned int cant_handlers_disp = 0;
    int *handlers_disp;         //Bandera para saber cuantos hay disponible
    int sig_handler;            //Utiliza el primer handler que se encuentra

    //Asignacion de memoria
    handlers_disp = malloc((unsigned long int)argumentos->max_clientes *sizeof(int));

    //Hilo para excribir archivo con bytes recibidos
    pthread_t file_writer_thread;
    struct local_writer_arg_struct file_writer_arg;

    strcpy(file_writer_arg.Write_File_Name, argumentos->INET_Write_File_Name);
    file_writer_arg.lock = &lock;
    file_writer_arg.salir = argumentos->salir;

    //Para configurar el socket se utiliza esta funcion
    server_conf_socket_INET(&listenfd, &serv_addr, argumentos->IPV4_iport, (unsigned long int)argumentos->max_clientes, argumentos->IPV4_Server_Address);

    //Iniciamos todos los handlers como disponibles
    for(int i=0; i<argumentos->max_clientes; i++)
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
        cant_handlers_disp = (long unsigned int) get_cant_hand_disp(handlers_disp,(unsigned long int) argumentos->max_clientes);
        pthread_mutex_unlock(&lock);    //Libero el lock

        //Compruebo que queden handlers disponibles
        //Usa get_prim_hand_disp para comprobar tambien que haya disponibles
        if(cant_handlers_disp == 0 || get_prim_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes) < 0)
        {
            while(cant_handlers_disp == 0)
            {
                pthread_mutex_lock(&lock);
                cant_handlers_disp = (long unsigned int)get_cant_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes);
                pthread_mutex_unlock(&lock);
            }
        }

        //Pasando la prueba de disponibilidad, agarra el primero disponible
        //Tambien en exclusion mutua
        pthread_mutex_lock(&lock);
        sig_handler = get_cant_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes);
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

        handler_thread_args[sig_handler].id = sig_handler;
        handler_thread_args[sig_handler].ConnSocket = &(connfd[sig_handler]);
        handler_thread_args[sig_handler].ExitThread = 0;
        handler_thread_args[sig_handler].Handlers = handlers_disp;
        handler_thread_args[sig_handler].lock = &lock;
        handler_thread_args[sig_handler].global_lock = argumentos->global_lock;
        handler_thread_args[sig_handler].salir = argumentos->salir;

        ocupar_handler(handlers_disp,sig_handler,&lock);

    }

    if(*(argumentos->salir) == 1)
    {
        for(int i=0; i<argumentos->max_clientes;i++)
        {
            pthread_mutex_lock(&lock);
            if(handlers_disp[i] == 1)
            {
                pthread_join(task_thread[i],NULL);
            }
            pthread_mutex_unlock(&lock);
        }
    }

    for(int i=0;i<argumentos->max_clientes;i++)
    {
        if((close(connfd[i])<0))
        {
            printf("Erro al cerrar conn %d\n",i);
            exit(EXIT_FAILURE);
        }
    }

    if((close(listenfd)<0)) 
    {
        printf("Error al cerrar listenfd\n");
        exit(EXIT_FAILURE); 
    }

    free(handlers_disp);   
    free(task_thread);          
    free(connfd);              
    free(handler_thread_args); 

    return NULL;

}

void server_conf_socket_INET(int *sock, struct sockaddr_in *serv_addr, int iport, long unsigned int max, char *string_addr)
{
    //Creacion de socket
    if((*(sock) = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        //Imprime error en caso de que asi sea
        printf("Error al crear el socket para el servidor\n");
        exit(EXIT_FAILURE);
    }

    //Matchea el path del archivo del server con el que se quiere conectar
    bzero(serv_addr, sizeof(*serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = inet_addr(string_addr);
    serv_addr->sin_port = htons((unsigned short int)iport);

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






