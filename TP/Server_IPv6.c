#include "Common.h"

void *Server_IPv6_codigo(void *arg)
{
    //VARIABLES
    //File descriptors
    int listenfd;                                           //Utilizado para escuchar conexiones
    int *connfd;                                            //Utilizado una vez ocurra el accept y haya conexion ya establecida
    
    //Estructuras
    struct sockaddr_in6 serv_addr;                          //Recuperar los argumentos
    struct IPv6_arg_struct *argumentos = arg;               //Para pasar la direccion del server
    struct local_threads_arg_struct *handler_thread_args;   //Para handlear las conexiones

    //Asignacion de memoria utilizando max_clientes como referencia
    connfd = malloc((unsigned long int) argumentos->max_clientes * sizeof(int));

    //HILOS
    pthread_t *task_thread;                                 //Tarea en si
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;       //Contador
    pthread_t *cont_thread;                                 //Para la exclusion mutua

    //Asignacion de memoria para los hilos utilizando cantidad maxima de clientes como referencia
    task_thread = malloc((unsigned long int) argumentos->max_clientes *sizeof(pthread_t));
    handler_thread_args = malloc((unsigned long int) argumentos->max_clientes * sizeof(struct local_threads_arg_struct));
    cont_thread = malloc((unsigned long int) argumentos->max_clientes * sizeof(pthread_t));

    //VARIABLES DE HANDLERS
    long unsigned int cant_handlers_disp = 0;               //Cantidad de handlers disponible
    int *handlers_disp;                                     //Array para saber que handler esta disponible
    int sig_handler;                                        //Utiliza el primer handler que se encuentra disponible

    //Asignacion de memoria utilizando max_clientes como referencia
    handlers_disp = malloc((unsigned long int)argumentos->max_clientes *sizeof(int));

    //Bytes recibidos por IPv6 (es decir, locales de este protocolo)
    unsigned long int bytes_total_recv_local;
    unsigned long int bytes_recv_ult_local;

    //Hilo y estructura para excribir archivo con bytes recibidos
    pthread_t file_writer_thread;
    struct local_writer_arg_struct file_writer_arg;

    //Se carga la estructura con los valores previamente creados
    file_writer_arg.bytes_recv_total = &bytes_total_recv_local;
    file_writer_arg.bytes_recv_ult = &bytes_recv_ult_local;
    strcpy(file_writer_arg.Write_File_Name, argumentos->IPv6_Write_File_Name);
    file_writer_arg.lock = &lock;
    file_writer_arg.salir = argumentos->salir;

    //Para configurar el socket se utiliza esta funcion
    server_conf_socket_IPv6(&listenfd, &serv_addr, argumentos->IPV6_iport, (unsigned long int)argumentos->max_clientes, argumentos->IPV6_Server_Address, argumentos->IPV6_Interface);

    //Iniciamos todos los handlers como disponibles
    for(int i=0; i<argumentos->max_clientes; i++)
    {
        handlers_disp[i] = 1;
    }

    //Crea y lanza un nuevo hilo que escribe el archivo local
    //Usa la funcion file_writing_thread que pasa un archivo que se abre de acuerdo a los argumentos que le pasamos
    pthread_create(&file_writer_thread,NULL,File_Writing_Thread_codigo,&file_writer_arg);

    //Esperando conexiones y lanzando hilos
    while(*(argumentos->salir) == 0)
    {
        //En exclusion mutua usando el lock
        pthread_mutex_lock(&lock);
        //Usa la funcion get_cant_hand_disp
        cant_handlers_disp = (long unsigned int) get_cant_hand_disp(handlers_disp,(unsigned long int) argumentos->max_clientes);
        pthread_mutex_unlock(&lock);                        //Libero el lock

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
        sig_handler = get_prim_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes);
        pthread_mutex_unlock(&lock);

        fflush(stdout);                                     //Limpia el buffer de stdout

        //Espera y acepta una conexion en el handler disponible
        connfd[sig_handler] = accept(listenfd, (SA*) NULL, NULL);

        //Si no hay handler siguiente, salta el error
        if(connfd[sig_handler] == -1)
        {
            printf("Error en accept:\n");
            exit(EXIT_FAILURE);
        }

        //Ya habiendo conexion, carga los argumentos de la estructura
        handler_thread_args[sig_handler].id = sig_handler;
        handler_thread_args[sig_handler].socket_conx = &(connfd[sig_handler]);
        handler_thread_args[sig_handler].thread_salida = 0;
        handler_thread_args[sig_handler].Handlers = handlers_disp;
        handler_thread_args[sig_handler].lock = &lock;
        handler_thread_args[sig_handler].global_lock = argumentos->global_lock;
        handler_thread_args[sig_handler].salir = argumentos->salir;
        handler_thread_args[sig_handler].total_bytes_recv_local = &bytes_total_recv_local;
        handler_thread_args[sig_handler].ult_bytes_recv_local = &bytes_recv_ult_local;
        handler_thread_args[sig_handler].total_bytes_recv_global = argumentos->total_bytes_recv_global;
        handler_thread_args[sig_handler].ult_bytes_recv_global = argumentos->ult_bytes_recv_global;
        handler_thread_args[sig_handler].segs = 0;

        //Se lanzan los hilos de la tarea en si y el contador
        pthread_create(&(task_thread[sig_handler]), NULL, Task, &(handler_thread_args[sig_handler]));
        pthread_create(&(cont_thread[sig_handler]), NULL, Contador_codigo, &(handler_thread_args[sig_handler]));

        //Se modifican los handlers disponibles en exclusion mutua
        ocupar_handler(handlers_disp,sig_handler,&lock);

    }

    //En caso de que se introduzca "salir", con join esperamos a terminen los hilos que estan corriendo
    if(*(argumentos->salir) == 1)
    {
        for(int i=0; i<argumentos->max_clientes;i++)
        {
            pthread_mutex_lock(&lock);
            if(handlers_disp[i] == 1)
            {
                pthread_join(task_thread[i],NULL);
                pthread_join(cont_thread[i],NULL);
                pthread_join(file_writer_thread,NULL);
            }
            pthread_mutex_unlock(&lock);
        }
    }

    //Recorremos y cerramos todos los file descriptors
    //CONEXION
    for(int i=0;i<argumentos->max_clientes;i++)
    {
        if((close(connfd[i])<0))
        {
            printf("Erro al cerrar conn %d\n",i);
            exit(EXIT_FAILURE);
        }
    }

    //ESCUCHA
    if((close(listenfd)<0)) 
    {
        printf("Error al cerrar listenfd\n");
        exit(EXIT_FAILURE); 
    }

    //Liberamos la memoria previamente asignada
    free(handlers_disp);   
    free(task_thread);          
    free(connfd);              
    free(handler_thread_args); 
    free(cont_thread);

    return NULL;

}

