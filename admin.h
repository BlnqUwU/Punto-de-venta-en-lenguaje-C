/*#ifndef ADMIN_H
#define ADMIN_H
#include "listas.h"
#include "utilidades.h"
#include "conexionipc.h"

typedef usuario * admin;
typedef articulo * producto;

//_____________________________________________________________________________________________

//                                              PROTOTIPOS

//_____________________________________________________________________________________________

// ──────────────────────────────────────────
// VERIFICACIONES INTERNAS
// ──────────────────────────────────────────

void crearAdminSiNoExiste();
int VerificarCorreo(char *correo);

//_____________________________________________________________________________________________

//                                              UI

//_____________________________________________________________________________________________

void menu();
void iniciarSesion();
void MenuPrincipal(char *usuario);
void AdministrarUsuarios();
void AdministrarCatalogo();
void GenerarReportes();

#endif */

#ifndef ADMIN_H
#define ADMIN_H
#include "listas.h"
#include "utilidades.h"
#include "conexionipc.h"

//typedef usuario * admin;
//
// typedef articulo * producto;

//_____________________________________________________________________________________________

//                                              PROTOTIPOS

//_____________________________________________________________________________________________

// ──────────────────────────────────────────
// VERIFICACIONES INTERNAS
// ──────────────────────────────────────────

void crearAdminSiNoExiste();
int VerificarCorreo(char *correo);

// ──────────────────────────────────────────
// CAMBIO: prototipos agregados
// ──────────────────────────────────────────

int buscarAdmin(char *usr);
int registrarAdmin(usuario a);
int solicitarSesionAdmin(usuario a);
int SolicitarSesion(usuario a);

int RegistrarUsuario(usuario u, char *arch);
int ModificarAtributo(usuario u, char *usr);
int ModificarAtributoCatalogo(articulo p, char *nombre);
int BorrarUsuario(usuario a, char *usr);

lista ObtenerUsuarios();
listaarticulo ObtenerProductos();
int RegistrarProducto(articulo p, char *arch);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
int BuscarProducto(char *producto);
void cargarVentasPorRango(listaventa lv, int dias);

//_____________________________________________________________________________________________

//                                              UI

//_____________________________________________________________________________________________

void menu();
void iniciarSesion();
void ServidorSinConexion();
void MenuPrincipal(char *usuario);
void AdministrarUsuarios();
void AdministrarCatalogo();
void GenerarReportes();

#endif