all:server client
server: server.o Myepoll.o socket.o fileoperate.o MD5.o
client: client.o dbtime.o socket.o fileoperate.o MD5.o
	gcc -o server server.o Myepoll.o socket.o fileoperate.o MD5.o
	gcc -o client client.o dbtime.o socket.o fileoperate.o MD5.o

server.o:server.c Myepoll.c Myepoll.h socket.c socket.h fileoperate.c fileoperate.h MD5.c MD5.h
	gcc -c server.c Myepoll.c socket.c fileoperate.c MD5.c

client.o:client.c dbtime.c dbtime.h socket.c socket.h fileoperate.c fileoperate.h MD5.c MD5.h
	gcc -c client.c socket.c fileoperate.c MD5.c

Myepoll.o:Myepoll.c Myepoll.h
	gcc -c Myepoll.c

socket.o:socket.c socket.h
	gcc -c socket.c

fileoperate.o:fileoperate.c fileoperate.h
	gcc -c fileoperate.c

dbtime.o:dbtime.c dbtime.h
	gcc -c dbtime.c

MD5.o:MD5.c MD5.h
	gcc -c MD5.c

clean:
	rm server client server.o client.o Myepoll.o dbtime.o socket.o fileoperate.o MD5.o
