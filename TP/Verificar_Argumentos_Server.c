#include "Common.h"

void verificar_argumentos(int argc, char *argv[])
{
    if(argc!=12)
    {
        printf("Cantidad de argumentos invalida\n");
        exit(EXIT_FAILURE);
    }

    Verificar_Argumentos_UNIX(argv);
    Verificar_Argumentos_IPv4(argv);
    Verificar_Argumentos_IPv6(argv);

    for(unsigned int i = 0; i < strlen(argv[10]); i ++)
    {
        if((isdigit(argv[10][i]) == 0)|| (atoi(argv[10]) < 0) || (atoi(argv[10]) > 10000) || strlen(argv[10]) > sizeof(int)) // Verificar que no se hayan ingresado letras, 
        {
            printf("Debe ingresar un puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }

    if((strlen(argv[11])>MAXLINE) || (!filename_valido(argv[11])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }
}

void Verificar_Argumentos_UNIX(char *argv[])
{
    if((strlen(argv[4])>MAXLINE) || (!filename_valido(argv[4])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    } 

    if((strlen(argv[5])>MAXLINE) || (!filename_valido(argv[5])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }
}

void  Verificar_Argumentos_IPv4(char *argv[])
{
    if(!dir_IPv4_valida(argv[1]))
    {
        printf("Debe ingresar una direccion IPv4 correcta\n");
        exit(EXIT_FAILURE);
    }

    for(unsigned int i = 0; i < strlen(argv[2]); i++)
    {
        if((isdigit(argv[2][i]) == 0) || (atoi(argv[2]) <= 0) || (atoi(argv[2])>65535))
        {
            printf("Debe ingresar un puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }

    if((strlen(argv[3])>MAXLINE) || (!filename_valido(argv[3])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }
}

void  Verificar_Argumentos_IPv6(char *argv[])
{
    if(!dir_IPv6_valida(argv[6]))
    {
        printf("Debe ingresar una direccion IPv6 valida\n");
        exit(EXIT_FAILURE);
    }

    for(unsigned int i = 0; i < strlen(argv[7]); i++)
    {
        if((isdigit(argv[7][i]) == 0) || (atoi(argv[7]) <= 0) || (atoi(argv[7]) > 65535))
        {
            printf("Debe ingresar un puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }
    for(unsigned int i = 0; i < strlen(argv[8]); i ++)
    {
        if((isdigit((argv[8][i]) == 0) && (isalpha(argv[8][i]) == 0)) || strlen(argv[8])>MAXLINE) 
        {                                                               
            printf("Debe ingresar una interfaz correcta\n");               
            exit(EXIT_FAILURE);
        }
    }

    if((strlen(argv[9])>MAXLINE) || (!filename_valido(argv[9])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }
}

