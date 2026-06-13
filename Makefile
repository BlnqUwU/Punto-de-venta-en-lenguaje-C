all: servidor admin cliente

servidor: servidor.c funciones_servidor.c listas.c utilidades.c
	gcc servidor.c funciones_servidor.c listas.c utilidades.c -o servidor -pthread -lncurses

cliente: cliente.c funciones_cliente.c listas.c conexionipc.c utilidades.c
	gcc cliente.c funciones_cliente.c listas.c conexionipc.c utilidades.c -o cliente -lncurses

admin: admin.c funciones_admin.c listas.c conexionipc.c utilidades.c
	gcc admin.c funciones_admin.c listas.c conexionipc.c utilidades.c -o admin -lncurses

clear:
	rm cliente admin servidor
