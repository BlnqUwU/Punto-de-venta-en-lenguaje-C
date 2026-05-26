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

void cargarCatalogo(listaarticulo cat);

// ──────────────────────────────────────────
// VENTAS
// ──────────────────────────────────────────

void actualizarExistencias(const char *nombreProducto, int cantidadVendida);
void registrarVenta(float total, const char *usr);
void devolverExistencias(const char *nombreProducto, int cantidad);

// ──────────────────────────────────────────
// USUARIOS
// ──────────────────────────────────────────
int cargarUsuario(usuario u);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
int RegistrarUsuario(usuario u,char *arch);
int SolicitarSesion(usuario u);
int ComprobarPassword(char *pass);
usuario SolicitarPerfil(char *usr);
int ModificarAtributo(usuario u, char *usr);

#endif