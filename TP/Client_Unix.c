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
#include <ctype.h>

#define SA struct sockaddr
#define MAXLINE 4096

//Funciones
void verificar_argumentos(int argc, char *argv[]);      //Comprobar argumentos
int filename_valido(char *string);                      //Comprobar si el filename es valido

int main(int argc, char *argv)
{
    //Declaracion de variables
    int sockfd;          //File descriptor de socket
    struct sockaddr_un server_addr;     //Struct para especificar server address

    //Para obtener mensaje por stdin
    char string[MAXLINE];       //Ingresado en stdin
    char aux[MAXLINE];      
    char fin_de_msg[7] = "\n";

    //Llamamos a la verificadora de argumentos
    verificar_argumentos(argc,argv);

    //Creacion de socket
    /*
    -UNIX
    -Stream
    -TCP
    */
    if((sockfd = socekt(AF_UNIX,SOCK_STREAM,0))<0)
    {
        printf("Fallo al crear socket\n");
        exit(EXIT_FAILURE);
    }
    
    //Handlear el path del archivo del server con el que se quiere conectar
    memset((char*) &server_addr, '\0', sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path,argv[1]);

    //Conexion con el server
    if(connect(sockfd, (SA*) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("falla al conectar con el servidor\n");
        exit(EXIT_FAILURE);
    }


    
}

int filename_valido(char *string) 
{
    //Controla la existencia de los siguientes caracteres: \/:*?"<>|
    if((strchr(string,(int)'/') != NULL) || (strchr(string,(int)':') != NULL) || (strchr(string,(int)'*') != NULL) || (strchr(string,(int)'?') != NULL) || 
    (strchr(string,(int)'<') != NULL) || (strchr(string,(int)'>') != NULL) || (strchr(string,(int)'|') != NULL))
    {
        return 0;
    }
    return 1;
}

void verificar_argumentos(int argc, char *argv[])
{
    //La cantidad de argumentos debe ser 5
    if(argc != 5)
    {
        printf("Cantidad de argumentos invalida. Deberian ser 5\n");
        exit(EXIT_FAILURE);
    }

    //Verifica que el nombre del archivo sea correcto
    if((strlen(argv[1])>MAXLINE) || (filename_valido(argv[1])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }

    //Verifica que el mensaje solo tenga letras y numeros
    for(unsigned int i = 0; strlen(argv[2]); i++)
    {
        if(((isdigit(argv[2][i]) == 0) && (isalpha(argv[2][i]) == 0)) || strlen(argv[2]) > MAXLINE)
        {
            printf("Debe ingresar un mensaje que solo conste de letras y numeros\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la cantidad de veces que se quiere enviar el mensaje sea correct
    for(unsigned int i = 0; strlen(argv[3]); i++)
    {
        if((isdigit(argv[3][i]) == 0) || (atoi(argv[3]) <= 0))
        {
            printf("Debe ingresar una cantidad de veces correcta\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la cantidad de microsegundos a esperar antes de enviar sea correcta
    for(unsigned int i = 0; strlen(argv[4]); i++)
    {
        if((isdigit(argv[4][i]) == 0) || (atoi(argv[4]) <= 0))
        {
            printf("Debe ingresar una cantidad correcta\n");
            exit(EXIT_FAILURE);
        }
    }

}