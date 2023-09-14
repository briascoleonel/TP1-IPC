#include "Common.h"

//Funciones
void verificar_argumentos_Unix(int argc, char *argv[]);      //Comprobar argumentos

int main(int argc, char *argv[])
{
    //Declaracion de variables
    int sockfd;          //File descriptor de socket
    struct sockaddr_un server_addr;     //Struct para especificar server address

    //Para obtener mensaje por stdin
    char string[MAXLINE];       //Ingresado en stdin
    char aux[MAXLINE];      
    char fin_de_msg[7] = "\n";

    long unsigned int veces_enviado;
    long unsigned int cont = 0;
    long unsigned int cant_bytes;
    long int escr_ret_val;

    //Llamamos a la verificadora de argumentos
    verificar_argumentos_Unix(argc,argv);

    //Creacion de socket
    /*
    -UNIX
    -Stream
    -TCP
    */
    if((sockfd = socket(AF_UNIX,SOCK_STREAM,0))<0)
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

    //Mensaje recibido por stdin
    strcpy(string,argv[2]);
    //Veces que se va a enviar
    veces_enviado = (unsigned long int)atoi(argv[3]);

    while(1)
    {
        if(cont != veces_enviado)   //Controlamos que se envie las veces requeridas
        {
            //contruccion del mensaje
            string[strcspn(string,"\n")] = 0;       //Saca el \n
            strcpy(aux,string);                 //Guarda en el aux
            strcat(string,fin_de_msg);          //Concatena el string con fin_de_msg

            //Envio del mensaje
            cant_bytes = strlen(string);        //Guarda la cantidad de bytes
            escr_ret_val = write(sockfd,string, cant_bytes);        //Devuelve cantidad de bytes escritos o -1 si falla
            //Comprobamos que no devuelva -1 o haya llegado a la cantidad de bytes    
            if((escr_ret_val == -1) || ((long unsigned int)escr_ret_val != cant_bytes))
            {
                printf("Fallo al enviar mensaje\n");
                exit(EXIT_FAILURE);
            }
            cont++;     //Aumenta el contador
        }
        else
        {
            break;
        }
    }

    close(sockfd);              //Cierra el socket
    exit(EXIT_SUCCESS);   
}

void verificar_argumentos_Unix(int argc, char *argv[])
{
    //La cantidad de argumentos debe ser 5
    if(argc != 5)
    {
        printf("Cantidad de argumentos invalida. Deberian ser 5\n");
        exit(EXIT_FAILURE);
    }

    //Verifica que el nombre del archivo sea correcto
    if((strlen(argv[1])>MAXLINE) || (!filename_valido(argv[1])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }

    //Verifica que el mensaje solo tenga letras y numeros
    for(unsigned int i = 0;i < strlen(argv[2]); i++)
    {
        if(((isdigit(argv[2][i]) == 0) && (isalpha(argv[2][i]) == 0)) || strlen(argv[2]) > MAXLINE)
        {
            printf("Debe ingresar un mensaje que solo conste de letras y numeros\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la cantidad de veces que se quiere enviar el mensaje sea correct
    for(unsigned int i = 0;i < strlen(argv[3]); i++)
    {
        if((isdigit(argv[3][i]) == 0) || (atoi(argv[3]) <= 0))
        {
            printf("Debe ingresar una cantidad de veces correcta\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la cantidad de microsegundos a esperar antes de enviar sea correcta
    for(unsigned int i = 0;i < strlen(argv[4]); i++)
    {
        if((isdigit(argv[4][i]) == 0) || (atoi(argv[4]) <= 0))
        {
            printf("Debe ingresar una cantidad correcta\n");
            exit(EXIT_FAILURE);
        }
    }

}