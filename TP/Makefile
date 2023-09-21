HEADERS = Common.h Handler.h Contador.h File_Writing_Thread.h Config_Socket_Unix.h Config_Socket_IPv4.h Config_Socket_IPv6.h Server_Unix.h Server_IPv4.h Server_IPv6.h Verificar_Argumentos_Server.h
OBJFILES = Common.o Handler.o Contador.o File_Writing_Thread.o Config_Socket_Unix.o Config_Socket_IPv4.o Config_Socket_IPv6.o Server_Unix.o Server_IPv4.o Server_IPv6.o Verificar_Argumentos_Server.o

Server: Main_Server.o $(OBJFILES) $(HEADERS)
	gcc -o Server Main_Server.o $(OBJFILES) -lpthread

Client: Client_IPv4.o $(OBJFILES) $(HEADERS)
	gcc -o Client_IPv4 Client_IPv4.o Common.o

Common.o: Common.c 
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lrt Common.c 

Handler.o: Handler.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Handler.c 

Contador.o: Contador.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Contador.c 

File_Writing_Thread.o: File_Writing_Thread.c 
	gcc -c -g -Wall -lpthread -lrt -pedantic -Werror -Wextra -Wconversion -std=gnu11 File_Writing_Thread.c 

Config_Socket_Unix.o: Config_Socket_Unix.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_Unix.c

Config_Socket_IPv4.o: Config_Socket_IPv4.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_IPv4.c

Config_Socket_IPv6.o: Config_Socket_IPv6.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_IPv6.c

Server_Unix.o: Server_Unix.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Server_Unix.c

Server_IPv4.o: Server_IPv4.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Server_IPv4.c

Server_IPv6.o: Server_IPv6.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Server_IPv6.c

Verificar_Argumentos_Server.o: Verificar_Argumentos_Server.c
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Verificar_Argumentos_Server.c

Client_Unix.o: Client_Unix.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Client_Unix.c 

Client_IPv4.o: Client_IPv4.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Client_IPv4.c 

Client_IPv6.o: Client_IPv6.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Client_IPv6.c 

all: Client_Unix.o Client_IPv4.o Client_IPv6.o Main_Server.o $(OBJFILES) $(HEADERS)
	gcc -o Client_Unix Client_Unix.o $(OBJFILES) -lpthread -lrt
	gcc -o Client_IPv4 Client_IPv4.o $(OBJFILES) -lpthread -lrt
	gcc -o Client_IPv6 Client_IPv6.o $(OBJFILES) -lpthread -lrt
	gcc -o Server Main_Server.o $(OBJFILES) -lpthread -lrt

clean:
	rm -f Server Client_Unix Client_IPv4 Client_IPv6 Server_INET_Stream UnixCon *.o *.txt

testServer: 
	./Server 127.0.0.1 1800 Bytes_IPv4.txt UnixCon Bytes_Unix.txt ::1 1800 enp2s0 Bytes_IPv6.txt 15 Bytes_Global.txt

testIPv4:
	./Client_IPv4 127.0.0.1 1800 mensaje 10 250000 &

testIPv6:
	./Client_IPv6 ::1 1800 enp2s0 mensaje 10 250000 &

testUnix:
	./Client_Unix UnixCon mensaje 10 250000 &
