#ifndef SERVIDOR_H
#define SERVIDOR_H
#include "listas.h"
#include "memoria_compartida.h"
#include "utilidades.h"

// ──────────────────────────────────────────
// INDICES DE SEMAFOROS
// SEM_INV -- protege lectura/escritura del inventario
// SEM_REQ -- cliente hizo una peticion
// SEM_ACK -- servidor respondio
// ──────────────────────────────────────────

enum { SEM_INV = 0, SEM_REQ = 1, SEM_ACK = 2 };

// ──────────────────────────────────────────
// UNION SEMUN
// ──────────────────────────────────────────

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

// ──────────────────────────────────────────
// FUNCIONES DE COMUNICACION
// ──────────────────────────────────────────
int BuscarAtributo(char *atributo, char *BD);
int EnviarSesion(usuario u, char *BD);
listaarticulo EnviarCatalogo();
lista EnviarUsuarios();

// ──────────────────────────────────────────
// FUNCIONES DE BASE DE DATOS (ARCHIVOS)
// ──────────────────────────────────────────
void CRUDusuario(lista usuarios, int CRUD);
void CRUDcatalogo(listaarticulo catalogo, int CRUD);
void CRUDventas(listaventa ventas, int CRUD);


#endif