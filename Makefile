all:server client
server: server.o Myepoll.o
client: client.o dbtime.o
	gcc -o server server.o Myepoll.o
	gcc -o client client.o dbtime.o

server.o:server.c Myepoll.c Myepoll.h
	gcc -c server.c Myepoll.c

client.o:client.c dbtime.c dbtime.h
	gcc -c client.c

Myepoll.o:Myepoll.c Myepoll.h
	gcc -c Myepoll.c

dbtime.o:dbtime.c dbtime.h
	gcc -c dbtime.c

clean:
	rm server client server.o client.o Myepoll.o dbtime.o
