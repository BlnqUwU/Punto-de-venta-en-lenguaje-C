#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "utilidades.h"

// ──────────────────────────────────────────
// CONSTANTES
// ──────────────────────────────────────────

#define MAX_PRODUCTOS  50
#define MAX_VENTAS     100
#define PERMISOS       0666
#define ARCHIVO_INV    "inventario.dat"
#define ARCHIVO_IPC    "IPCKeys"
#define XOR_KEY        0x5A

// ──────────────────────────────────────────
// STRUCTS
// ──────────────────────────────────────────

typedef struct {
    int   id;
    char  nombre[64];
    float precio;
    int   existencias;
    int   activo;   // 1 = existe, 0 = eliminado
} Producto;

typedef struct {
    float total;
    char  usr[100];
} Venta;

typedef struct {
    Producto productos[MAX_PRODUCTOS];
    int      totalProductos;
    Venta    ventas[MAX_VENTAS];
    int      totalVentas;
} InventarioShm;

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
// PROTOTIPOS — ARCHIVO ENCRIPTADO
// ──────────────────────────────────────────

void encriptar(const char *entrada, const char *salida);
void desencriptar(const char *entrada, const char *salida);
int  guardarInventario(InventarioShm *shm);
int  cargarInventario(InventarioShm *shm);

// ──────────────────────────────────────────
// PROTOTIPOS — INVENTARIO
// ──────────────────────────────────────────

int agregarProducto(InventarioShm *shm, Producto p);
int eliminarProducto(InventarioShm *shm, int id);
int modificarProducto(InventarioShm *shm, int id, Producto nuevo);
int buscarProducto(InventarioShm *shm, int id);
int venderProducto(InventarioShm *shm, int id, int cantidad, const char *usr);

#endif