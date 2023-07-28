
all:common.o tcpc tcps

common.o: common.h common.c
	gcc -c common.c

tcpc: tcpclient.c
	gcc -o tcpc tcpclient.c common.o

tcps: tcpserver.c
	gcc -o tcps tcpserver.c common.o


clean:
	rm -rf *.dSYM tcpc
 
