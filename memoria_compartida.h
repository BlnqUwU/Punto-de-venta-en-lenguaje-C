#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

#include "utilidades.h"
#include "listas.h"

// ──────────────────────────────────────────
// CONSTANTES
// ──────────────────────────────────────────

#define PERMISOS       0666
#define ARCHIVO_CAT    "catalogo.txt"
#define ARCHIVO_USR    "usuarios.txt"
#define ARCHIVO_VD    "ventas_diarias.txt"
#define ARCHIVO_VS    "ventas_semanales.txt"
#define ARCHIVO_VM    "ventas_mensuales.txt"
#define ARCHIVO_IPC    "IPCKeys"

// ──────────────────────────────────────────
// Datos en memoria compartida
// ──────────────────────────────────────────

typedef struct {
    listaarticulo catalogo;
    articulo p;
    int CRUD;
    int realizado;
} InventarioShm;

typedef struct {
    lista usuarios;
    usuario u;
    char *atributo;
    int CRUD;
    int realizado;
} usuarioShm;

typedef struct {
    listaventa ventas;
    venta v;
    int CRUD;
    int realizado;
} ventaShm;


// ──────────────────────────────────────────
// PROTOTIPOS — ARCHIVO ENCRIPTADO
// ──────────────────────────────────────────

//void encriptar(const char *entrada, const char *salida);
//void desencriptar(const char *entrada, const char *salida);
//int  guardarInventario(InventarioShm *shm);
//int  cargarInventario(InventarioShm *shm);

// ──────────────────────────────────────────
// PROTOTIPOS — INVENTARIO
// ──────────────────────────────────────────
/*
int agregarProducto(InventarioShm *shm, Producto p);
int eliminarProducto(InventarioShm *shm, int id);
int modificarProducto(InventarioShm *shm, int id, Producto nuevo);
int buscarProducto(InventarioShm *shm, int id);
int venderProducto(InventarioShm *shm, int id, int cantidad, const char *usr);
*/
#endif