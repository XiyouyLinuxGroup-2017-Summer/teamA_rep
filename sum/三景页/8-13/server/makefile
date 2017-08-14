main:server.o my_recv.o persist.o
	gcc -o server server.o my_recv.o persist.o -pthread -L/usr/lib64/mysql -lmysqlclient
server.o:server.c
	gcc -c server.c
my_recv.o:my_recv.c my_recv.h
	gcc -c my_recv.c
persist.o:persist.c persist.h
	gcc -c persist.c
