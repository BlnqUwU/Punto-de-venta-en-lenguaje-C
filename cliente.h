#ifndef CLIENTE_H
#define CLIENTE_H
#include "utilidades.h"
#include "listas.h"

//_____________________________________________________________________________________________

//                                              PROTOTIPOS

//_____________________________________________________________________________________________

// ──────────────────────────────────────────
// VERIFICACIONES INTERNAS
// ──────────────────────────────────────────

int VerificarCorreo(char *correo);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
int ComprobarPassword(char *pass);

//_____________________________________________________________________________________________

//                                              UI

//_____________________________________________________________________________________________

void registrar();
void iniciarSesion();
void menu();
void MenuPrincipal(usuario u);
void Catalogo(usuario u);
void Carrito(usuario u);
int Perfil(usuario u);

#endif
