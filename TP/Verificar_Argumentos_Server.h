#ifndef SERVERARGVERIFY_H    
#define SERVERARGVERIFY_H_H

void verificar_argumentos(int argc, char *argv[]);
void Verificar_Argumentos_UNIX(char *argv[]);
void Verificar_Argumentos_IPv4(char *argv[]);
void Verificar_Argumentos_IPv6(char *argv[]);

#endif