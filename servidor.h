#ifndef SERVIDOR_H
#define SERVIDOR_H
#include "listas.h"
#include "memoria_compartida.h"
#include "utilidades_back.h"

// ──────────────────────────────────────────
// INDICES DE SEMAFOROS
// SEM_INV -- protege lectura/escritura del inventario
// SEM_USR -- protege lectura/escritura de Usuarios
// SEM_VTA -- protege lectura/escritura de Ventas
// SEM_REQ -- cliente hizo una peticion
// SEM_ACK -- servidor respondio
// ──────────────────────────────────────────

enum { SEM_INV = 0, SEM_USR = 1, SEM_VTA = 2, SEM_REQ = 3, SEM_ACK = 4 };

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
void CRUDusuario(usuarioShm *Ushm, int CRUD);
void CRUDcatalogo(InventarioShm *Ishm, int CRUD, int BD);
void CRUDventas(ventaShm *Vshm, int CRUD);


#endif