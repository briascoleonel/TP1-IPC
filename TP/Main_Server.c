#include "Common.h"

int main(int argc, char*argv[])
{
    pthread_t UNIX_Server_Thread;
    pthread_t IPv4_Server_Thread;
    pthread_t IPv6_Server_Thread;

    struct UNIX_arg_struct UNIX_argumentos;
    struct IPv4_arg_struct IPv4_argumentos;
    struct IPv6_arg_struct IPv6_argumentos;

    pthread_t Global_File_Writing_Thread;
    struct local_writer_arg_struct global_argumentos;

    pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

    char salir[MAXLINE];
    int salir_todos = 0;

    verificar_argumentos(argc,argv);

    strcpy(UNIX_argumentos.UNIX_File_Name,argv[4]);
    strcpy(UNIX_argumentos.UNIX_Write_File_Name,argv[5]);
    UNIX_argumentos.global_lock = &global_lock;
    UNIX_argumentos.max_clientes = argv[10];
    UNIX_argumentos.salir = &salir_todos;

    strcpy(IPv4_argumentos.IPV4_Server_Address,argv[1]);
    IPv4_argumentos.IPV4_iport = (short unsigned int)atoi(argv[2]);
    strcpy(IPv4_argumentos.IPv4_Write_File_Name,argv[3]);
    IPv4_argumentos.global_lock = &global_lock;
    IPv4_argumentos.max_clientes = atoi(argv[10]);
    IPv4_argumentos.salir = &salir_todos;

    strcpy(IPv6_argumentos.IPV6_Server_Address,argv[6]);
    IPv6_argumentos.IPV6_iport = (short unsigned int)atoi(argv[7]);
    strcpy(IPv6_argumentos.IPV6_Interface,argv[8]);
    strcpy(IPv6_argumentos.IPv6_Write_File_Name,argv[9]);
    IPv6_argumentos.global_lock = &global_lock;
    IPv6_argumentos.max_clientes = atoi(argv[10]);
    IPv6_argumentos.salir = &salir_todos;

    strcpy(global_argumentos.Write_File_Name,argv[11]);
    global_argumentos.lock = &global_lock;
    global_argumentos.salir = &salir_todos;

    //Tenemos que cambiar las funciones de server
    pthread_create(&Global_File_Writing_Thread,NULL,File_Writing_Thread_codigo, &global_argumentos);
    pthread_create(&UNIX_Server_Thread,NULL,Server_Unix_codigo, &UNIX_argumentos);
    pthread_create(&IPv4_Server_Thread,NULL,Server_IPv4_codigo, &IPv4_argumentos);
    pthread_create(&IPv6_Server_Thread,NULL,Server_IPv6_codigo, &IPv6_argumentos);

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