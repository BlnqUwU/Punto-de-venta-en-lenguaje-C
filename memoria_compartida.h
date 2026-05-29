#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

#include "utilidades.h"
#include "listas.h"

// ──────────────────────────────────────────
// CONSTANTES
// ──────────────────────────────────────────

#define PERMISOS       0666
#define MAX_PRODUCTOS  50
#define MAX_USUARIOS   100
#define MAX_VENTAS     500
#define ARCHIVO_CAT    "catalogo.dat"
#define ARCHIVO_USR    "usuarios.dat"
#define ARCHIVO_VD     "ventas_diarias.txt"
#define ARCHIVO_VS     "ventas_semanales.txt"
#define ARCHIVO_VM     "ventas_mensuales.txt"
#define ARCHIVO_IPC    "IPCKeys"

// ──────────────────────────────────────────
// CONTROL — indica al servidor qué shm atender
// ──────────────────────────────────────────

typedef struct {
    int tipo; // Inventario = 0, Usuario = 1, Venta = 2
} ControlShm;


// ──────────────────────────────────────────
// Datos en memoria compartida
// ──────────────────────────────────────────

typedef struct {
    articulo catalogo[MAX_PRODUCTOS];
    int      totalCatalogo;
    articulo p;         // articulo para operacion individual
    int      CRUD;      // 0=crear 1=leer 2=actualizar 3=borrar
    int      realizado; // 0=pendiente 1=exito -1=error
} InventarioShm;


typedef struct {
    usuario usuarios[MAX_USUARIOS];
    int     totalUsuarios;
    usuario u;          // usuario para operacion individual
    char    usr_original[100];
    char    atributo[100];
    int     CRUD;
    int     realizado;
} usuarioShm;


typedef struct {
    venta ventas[MAX_VENTAS];
    int   totalVentas;
    venta v;            // venta para operacion individual
    int   tipo;         // 0=diario 1=semanal 2=mensual
    int   CRUD;
    int   realizado;
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