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

int checarservidor();
int VerificarCorreo(char *correo);
int ComprobarPassword(char *pass);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);

//_____________________________________________________________________________________________

//                                              UI

//_____________________________________________________________________________________________

void registrar();
void iniciarSesion();
void ServidorSinConexion();
void menu();
void MenuPrincipal(usuario u);
void Catalogo(usuario u);
void Carrito(usuario u);
int Perfil(usuario u);

#endif
