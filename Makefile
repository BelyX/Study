all:server client
server: server.o  socket.o fileoperate.o MD5.o
client: client.o  socket.o fileoperate.o MD5.o
	gcc -o server server.o  socket.o fileoperate.o MD5.o
	gcc -o client client.o  socket.o fileoperate.o MD5.o

server.o:server.c  socket.c socket.h fileoperate.c fileoperate.h MD5.c MD5.h
	gcc -c server.c  socket.c fileoperate.c MD5.c

client.o:client.c  socket.c socket.h fileoperate.c fileoperate.h MD5.c MD5.h
	gcc -c client.c socket.c fileoperate.c MD5.c

socket.o:socket.c socket.h
	gcc -c socket.c

fileoperate.o:fileoperate.c fileoperate.h
	gcc -c fileoperate.c

MD5.o:MD5.c MD5.h
	gcc -c MD5.c

clean:
	rm server client server.o client.o  socket.o fileoperate.o MD5.o
