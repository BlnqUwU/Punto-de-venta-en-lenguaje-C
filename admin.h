#ifndef ADMIN_H
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

#endif