#include "Common.h"

void verificar_argumentos(int argc, char *argv[])
{
    if(argc!=12)
    {
        printf("Cantidad de argumentos invalida\n");
        exit(EXIT_FAILURE);
    }

    Verificar_Argumentos_IPv4(argv);
    Verificar_Argumentos_UNIX(argv);
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