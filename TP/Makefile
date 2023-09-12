HEADERS = Common.h Handler.h Contador.h File_Writing_Thread.h Config_Socket_Unix.h Config_Socket_IPv4.h Config_Socket_IPv6.h Server_Unix.h Server_IPv4.h Server_IPv6.h Verificar_Argumentos_Server.h
OBJFILES = Common.o Handler.o Contador.o File_Writing_Thread.o Config_Socket_Unix.o Config_Socket_IPv4.o Config_Socket_IPv6.o Server_Unix.o Server_IPv4.o Server_IPv6.o Verificar_Argumentos_Server.o

Server: Main_Server.o $(OBJFILES) $(HEADERS)
	gcc -o Server Main_Server.o $(OBJFILES) -lpthread

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

clean:
	rm -f Server  *.o 