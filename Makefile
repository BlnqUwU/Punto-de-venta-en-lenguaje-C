cliente: cliente.c funciones_cliente.c cliente.h
	gcc cliente.c funciones_cliente.c cliente.h -o cliente -lncurses

admin: admin.c funciones_admin.c admin.h
	touch catalogo.txt
	gcc admin.c funciones_admin.c admin.h -o admin -lncurses
