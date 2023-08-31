#ifndef UNIXSOCKETCONFIG_H    
#define UNIXSOCKETCONFIG_H_H

void Config_Socket_Unix(int *sock, struct sockaddr_un *servaddr, long unsigned int max, char *filename);

#endif