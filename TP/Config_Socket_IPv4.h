#ifndef IPV4SOCKETCONFIG_H    
#define IPV4SOCKETCONFIG_H_H

void server_conf_socket_INET(int *sock, struct sockaddr_in *servaddr, int iport, long unsigned int max, char *stringaddr);

#endif 