#ifndef CONEXIONIPC_H
#define CONEXIONIPC_H

#include "memoria_compartida.h"
#include "listas.h"
#include "utilidades.h"

int  conectarServidor();
void desconectarServidor();

// ──────────────────────────────────────────
// FUNCIONES DE COMUNICACION
// ──────────────────────────────────────────

int enviararticulo(articulo p, int CRUD, int BD); // 0 catalogo, 1 carrito
int obtenerCantidadCatalogo(char *producto);
listaarticulo ObtenerCatalogo();
listaarticulo ObtenerCarrito();
void guardarCarrito(char *usr);
void cargarCarrito(char *usr);
void limpiarCarrito();


int enviarusuario(usuario u, int CRUD, char *nombreUsuario); //lee llaves primarias (usuario correo)
int enviarCorreo(usuario u, int CRUD);

int solicitarSesion(usuario u);
usuario obtenerUsuario(usuario u);
lista ObtenerUsuarios();

listaventa obtenerVentas(int tipo); //0 DIARIO, 1 SEMANAL, 2 MENSUAL
void enviarVenta(venta v);



#endif