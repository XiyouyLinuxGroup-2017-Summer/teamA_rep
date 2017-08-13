main:server.o my_recv.o
	gcc -o server server.o my_recv.o
server.o:server.c my_recv.h
	gcc -c server.c
my_recv.o:my_recv.c my_recv.h
	gcc -c my_recv.c
