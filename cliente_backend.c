#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "inventario.h"
#include "cliente.h"
#include "utilidades.h"

// ──────────────────────────────────────────
// CONEXION A IPC
// (el servidor ya creo shm y semaforos)
// ──────────────────────────────────────────

static InventarioShm *shm   = NULL;
static int            semID = -1;
static int            shmID = -1;

int conectarServidor() {
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

void desconectarServidor() {
    if (shm) shmdt(shm);
}

// ──────────────────────────────────────────
// 1. CARGAR CATALOGO DESDE SHM A LISTA
//    Sustituye: //insertar catalogo de memoria compartida a lista
// ──────────────────────────────────────────

void cargarCatalogo(lista cat) {

    if (!shm) {
        printf("DEBUG: shm es NULL\n");
        return;
    }
    printf("DEBUG: totalProductos = %d\n", shm->totalProductos);

    downSem(semID, SEM_INV);

    for (int i = 0; i < shm->totalProductos; i++) {
        Producto *p = &shm->productos[i];

        if (!p->activo || p->existencias <= 0)
            continue;

        info item;
        strncpy(item.producto, p->nombre, sizeof(item.producto) - 1);
        item.producto[sizeof(item.producto) - 1] = '\0';
        item.cantidad = p->existencias;
        item.precio   = p->precio;

        add(cat->NE, item, cat);
    }

    upSem(semID, SEM_INV);
}

// ──────────────────────────────────────────
// 2. ACTUALIZAR EXISTENCIAS EN SHM
//    Sustituye: //actualizar catalogo servidor
// ──────────────────────────────────────────

void actualizarExistencias(const char *nombreProducto, int cantidadVendida) {
    if (!shm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < shm->totalProductos; i++) {
        Producto *p = &shm->productos[i];

        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->existencias -= cantidadVendida;
            if (p->existencias < 0)
                p->existencias = 0;
            break;
        }
    }

    upSem(semID, SEM_INV);

    // NOTIFICAR AL SERVIDOR QUE HAY CAMBIOS
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

// ──────────────────────────────────────────
// 3. REGISTRAR VENTA EN SHM
//    Sustituye: //mandar totalpagar a servidor para reporte de venta
// ──────────────────────────────────────────

void registrarVenta(float total, const char *usr) {
    if (!shm) return;

    downSem(semID, SEM_INV);

    if (shm->totalVentas < MAX_VENTAS) {
        Venta v;
        v.total = total;
        strncpy(v.usr, usr, sizeof(v.usr) - 1);
        v.usr[sizeof(v.usr) - 1] = '\0';

        shm->ventas[shm->totalVentas] = v;
        shm->totalVentas++;
    }

    upSem(semID, SEM_INV);

    FILE *f = fopen("ventas.txt", "a");
    if (f) {
        fprintf(f, "%s,%.2f\n", usr, total);
        fclose(f);
    }

    // NOTIFICAR AL SERVIDOR PARA QUE GUARDE
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

void devolverExistencias(const char *nombreProducto, int cantidad) {
    if (!shm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < shm->totalProductos; i++) {
        Producto *p = &shm->productos[i];
        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->existencias += cantidad;
            break;
        }
    }

    upSem(semID, SEM_INV);

    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}