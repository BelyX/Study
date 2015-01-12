all:server client
server: server.o Myepoll.o socket.o fileoperate.o
client: client.o dbtime.o socket.o fileoperate.o
	gcc -o server server.o Myepoll.o socket.o fileoperate.o
	gcc -o client client.o dbtime.o socket.o fileoperate.o

server.o:server.c Myepoll.c Myepoll.h socket.c socket.h fileoperate.c fileoperate.h
	gcc -c server.c Myepoll.c socket.c fileoperate.c

client.o:client.c dbtime.c dbtime.h socket.c socket.h fileoperate.c fileoperate.h
	gcc -c client.c socket.c fileoperate.c

Myepoll.o:Myepoll.c Myepoll.h
	gcc -c Myepoll.c

socket.o:socket.c socket.h
	gcc -c socket.c

fileoperate.o:fileoperate.c fileoperate.h
	gcc -c fileoperate.c

dbtime.o:dbtime.c dbtime.h
	gcc -c dbtime.c

clean:
	rm server client server.o client.o Myepoll.o dbtime.o socket.o fileoperate.o
