#ifndef CLIENTE_BACKEND_H
#define CLIENTE_BACKEND_H

#include "cliente.h"

// ──────────────────────────────────────────
// CONEXION AL SERVIDOR
// ──────────────────────────────────────────

int  conectarServidor();
void desconectarServidor();

// ──────────────────────────────────────────
// CATALOGO
// ──────────────────────────────────────────

void cargarCatalogo(lista cat);

// ──────────────────────────────────────────
// VENTAS
// ──────────────────────────────────────────

void actualizarExistencias(const char *nombreProducto, int cantidadVendida);
void registrarVenta(float total, const char *usr);
void devolverExistencias(const char *nombreProducto, int cantidad);

#endif