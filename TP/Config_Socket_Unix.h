#ifndef UNIXSOCKETCONFIG_H    // Comienzo del ifdef guard
#define UNIXSOCKETCONFIG_H_H

void server_conf_socket_Unix(int *sock, struct sockaddr_un *serv_addr, long unsigned int max, char *filename);

#endif