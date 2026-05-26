servidor: funciones_servidor.c funciones_admin.c utilidades.c 
	gcc servidor.c funciones_servidor.c funciones_admin.c utilidades.c  -o servidor -pthread
cliente: cliente.c funciones_cliente.c cliente_backend.c utilidades.c
	gcc cliente.c funciones_cliente.c cliente_backend.c utilidades.c -o cliente -lncurses
admin: funciones_servidor.c funciones_admin.c utilidades.c admin_backend.c
	gcc admin.c funciones_servidor.c funciones_admin.c utilidades.c admin_backend.c  -o admin -lncurses
