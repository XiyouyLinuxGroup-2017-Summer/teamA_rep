main:client.o my_recv.o view.o logic.o
	gcc -o client client.o my_recv.o view.o logic.o
client.o:client.c my_recv.h view.h logic.h
	gcc -c client.c
my_recv.o:my_recv.c my_recv.h
	gcc -c my_recv.c
view.o:view.c view.h
	gcc -c view.c
logic.o:logic.c logic.h
	gcc -c logic.c
