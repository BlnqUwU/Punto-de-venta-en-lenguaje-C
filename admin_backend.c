#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "inventario.h"
#include "admin.h"
#include "utilidades.h"

// ──────────────────────────────────────────
// CONEXION A IPC
// ──────────────────────────────────────────

static InventarioShm *shm   = NULL;
static int            semID = -1;
static int            shmID = -1;

int conectarServidorAdmin() {
    FILE *f = fopen(ARCHIVO_IPC, "a");
    if (f) fclose(f);

    key_t keyShm = ftok(ARCHIVO_IPC, 'M');
    key_t keySem = ftok(ARCHIVO_IPC, 'S');
    if (keyShm == -1 || keySem == -1) {
        perror("ftok");
        return 0;
    }

    shmID = shmget(keyShm, sizeof(InventarioShm), PERMISOS);
    if (shmID == -1) {
        perror("shmget — asegurate de que el servidor este corriendo");
        return 0;
    }

    shm = (InventarioShm *) shmat(shmID, NULL, 0);
    if (shm == (void *) -1) {
        perror("shmat");
        return 0;
    }

    semID = semget(keySem, 3, PERMISOS);
    if (semID == -1) {
        perror("semget");
        return 0;
    }

    return 1;
}

void desconectarServidorAdmin() {
    if (shm) shmdt(shm);
}

// ──────────────────────────────────────────
// CARGAR CATALOGO DESDE SHM A LISTAPRODUCTO
// ──────────────────────────────────────────

void cargarCatalogoAdmin(listaproducto cat) {
    if (!shm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < shm->totalProductos; i++) {
        Producto *p = &shm->productos[i];
        if (!p->activo) continue;

        producto item;
        strncpy(item.producto, p->nombre, sizeof(item.producto) - 1);
        item.producto[sizeof(item.producto) - 1] = '\0';
        item.cantidad = p->existencias;
        item.precio   = p->precio;

        prod fin;
        fin.p = item;
        addproducto(cat->NE, fin, cat);
    }

    upSem(semID, SEM_INV);
}

// ──────────────────────────────────────────
// AGREGAR PRODUCTO A SHM
// ──────────────────────────────────────────

int agregarProductoAdmin(prod p) {
    if (!shm) return 0;

    Producto nuevo;
    strncpy(nuevo.nombre, p.p.producto, sizeof(nuevo.nombre) - 1);
    nuevo.nombre[sizeof(nuevo.nombre) - 1] = '\0';
    nuevo.precio       = p.p.precio;
    nuevo.existencias  = p.p.cantidad;
    nuevo.activo       = 1;

    downSem(semID, SEM_INV);
    int ok = agregarProducto(shm, nuevo);
    upSem(semID, SEM_INV);

    if (ok) {
        upSem(semID, SEM_REQ);
        downSem(semID, SEM_ACK);
    }

    return ok;
}

// ──────────────────────────────────────────
// MODIFICAR EXISTENCIAS EN SHM
// ──────────────────────────────────────────

void modificarExistenciasAdmin(const char *nombreProducto, int nuevaCantidad) {
    if (!shm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < shm->totalProductos; i++) {
        Producto *p = &shm->productos[i];
        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->existencias = nuevaCantidad;
            break;
        }
    }

    upSem(semID, SEM_INV);

    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

// ──────────────────────────────────────────
// ELIMINAR PRODUCTO EN SHM
// ──────────────────────────────────────────

void eliminarProductoAdmin(const char *nombreProducto) {
    if (!shm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < shm->totalProductos; i++) {
        Producto *p = &shm->productos[i];
        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->activo = 0;
            break;
        }
    }

    upSem(semID, SEM_INV);

    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}