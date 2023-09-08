#ifndef IPV6SOCKETCONFIG_H    
#define IPV6SOCKETCONFIG_H_H

void server_conf_socket_IPv6(int *sock, struct sockaddr_in6 *servaddr, int iport, long unsigned int max, char *stringaddr, char *interfaceName);

#endif 

