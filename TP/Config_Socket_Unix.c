#include "Common.h"

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

    int yes = 1;

    if(setsockopt(*(sock), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) ==-1 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

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