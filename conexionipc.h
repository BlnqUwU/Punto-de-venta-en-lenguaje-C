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

int enviararticulo(articulo p, int CRUD);
listaarticulo ObtenerCatalogo();

int enviarusuario(usuario u, int CRUD);
usuario obtenerUsuario(usuario u);
lista ObtenerUsuarios();

listaventa obtenerVentas();
void enviarVenta(venta v);



#endif