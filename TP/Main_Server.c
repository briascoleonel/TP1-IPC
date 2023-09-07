#include "Common.h"

int main(int argc, char*argv[])
{
    //Hilos y estructuras para los protocolos
    pthread_t UNIX_Server_Thread;
    pthread_t IPv4_Server_Thread;
    pthread_t IPv6_Server_Thread;

    struct UNIX_arg_struct UNIX_argumentos;
    struct IPv4_arg_struct IPv4_argumentos;
    struct IPv6_arg_struct IPv6_argumentos;

    //Hilo y estructura para hilo que se va a usar para bytes recibidos
    pthread_t Global_File_Writing_Thread;
    struct local_writer_arg_struct global_argumentos;

    //Lock para exclusion mutua
    pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

    //Mecanismo de salida
    char salir[MAXLINE];
    int salir_todos = 0;

    unsigned long int bytes_total_recv_glob;
    unsigned long int bytes_ult_recv_glob;

    //Funcion para verificar los argumentos de los 3 protocolos
    verificar_argumentos(argc,argv);

    //Asignacion de argumentos de acuerdo a la estructura de cada hilo
    
    //UNIX
    strcpy(UNIX_argumentos.UNIX_File_Name,argv[4]);
    strcpy(UNIX_argumentos.UNIX_Write_File_Name,argv[5]);
    UNIX_argumentos.global_lock = &global_lock;
    UNIX_argumentos.total_bytes_recv_global = &bytes_total_recv_glob;
    UNIX_argumentos.ult_bytes_recv_global = &bytes_ult_recv_glob;
    UNIX_argumentos.max_clientes = argv[10];
    UNIX_argumentos.salir = &salir_todos;

    //IPv4
    strcpy(IPv4_argumentos.IPV4_Server_Address,argv[1]);
    IPv4_argumentos.IPV4_iport = (short unsigned int)atoi(argv[2]);
    strcpy(IPv4_argumentos.IPv4_Write_File_Name,argv[3]);
    IPv4_argumentos.global_lock = &global_lock;
    IPv4_argumentos.total_bytes_recv_global = &bytes_total_recv_glob;
    IPv4_argumentos.ult_bytes_recv_global = &bytes_total_recv_glob;
    IPv4_argumentos.max_clientes = atoi(argv[10]);
    IPv4_argumentos.salir = &salir_todos;

    //IPv6
    strcpy(IPv6_argumentos.IPV6_Server_Address,argv[6]);
    IPv6_argumentos.IPV6_iport = (short unsigned int)atoi(argv[7]);
    strcpy(IPv6_argumentos.IPV6_Interface,argv[8]);
    strcpy(IPv6_argumentos.IPv6_Write_File_Name,argv[9]);
    IPv6_argumentos.global_lock = &global_lock;
    IPv6_argumentos.total_bytes_recv_global = &bytes_total_recv_glob;
    IPv6_argumentos.ult_bytes_recv_global = &bytes_ult_recv_glob;
    IPv6_argumentos.max_clientes = atoi(argv[10]);
    IPv6_argumentos.salir = &salir_todos;

    //Global (bytes recibidos)
    global_argumentos.bytes_recv_total = &bytes_total_recv_glob;
    global_argumentos.bytes_recv_ult = &bytes_ult_recv_glob;
    strcpy(global_argumentos.Write_File_Name,argv[11]);
    global_argumentos.lock = &global_lock;
    global_argumentos.salir = &salir_todos;

    //Creacion y lanzamiento de hilos
    pthread_create(&Global_File_Writing_Thread,NULL,File_Writing_Thread_codigo, &global_argumentos);
    pthread_create(&UNIX_Server_Thread,NULL,Server_Unix_codigo, &UNIX_argumentos);
    pthread_create(&IPv4_Server_Thread,NULL,Server_IPv4_codigo, &IPv4_argumentos);
    pthread_create(&IPv6_Server_Thread,NULL,Server_IPv6_codigo, &IPv6_argumentos);

    //Mecanismo de salida
    while(!salir_todos)
    {
        printf("Ingrese -salir- para cerrar el server\n");
        if(strcmp(salir,"salir"))
        {
            salir_todos = 1;
        }
        else
        {
            printf("No ha ingresado salir. Ha ingresado: %s\n",salir);
        }
    }

}