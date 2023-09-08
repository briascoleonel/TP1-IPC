#include "Common.h"

void server_conf_socket_IPv6(int *sock, struct sockaddr_in6 *serv_addr, int iport, long unsigned int max, char *string_addr, char *interface_name)
{
    //Creacion de socket
    if((*(sock) = socket(AF_INET6,SOCK_STREAM,0)) < 0)
    {
        //Imprime error en caso de que asi sea
        printf("Error al crear el socket para el servidor\n");
        exit(EXIT_FAILURE);
    }

    //Matchea el path del archivo del server con el que se quiere conectar
    bzero(serv_addr, sizeof(*serv_addr));
    serv_addr->sin6_family = AF_INET6;
    if(inet_pton(AF_INET6, string_addr, &(serv_addr->sin6_addr)) != 1)
    {
        printf("Direccion mal\n");
        exit(EXIT_FAILURE);
    }
    serv_addr->sin6_port = htons((unsigned short int)iport);
    serv_addr->sin6_scope_id = if_nametoindex(interface_name);

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

