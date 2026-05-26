#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include "inventario.h"
#include "cliente.h"
#include "utilidades.h"

// ──────────────────────────────────────────
// CONEXION A IPC
// (el servidor ya creo shm y semaforos)
// ──────────────────────────────────────────

static InventarioShm *Ishm   = NULL;
static usuarioShm *Ushm   = NULL;
static int            semID = -1;
static int            shmID = -1;
static int            shmID2 = -1;


int conectarServidor() {
    FILE *f = fopen(ARCHIVO_IPC, "a");
    if (f) fclose(f);

    key_t keyShm = ftok(ARCHIVO_IPC, 'M');
    key_t keyShm_usr = ftok(ARCHIVO_IPC, 'U');
    key_t keySem = ftok(ARCHIVO_IPC, 'S');
    if (keyShm == -1 || keySem == -1) {
        perror("ftok");
        return 0;
    }

    //Inventario
    shmID = shmget(keyShm, sizeof(InventarioShm), PERMISOS);
    if (shmID == -1) {
        perror("shmget — asegurate de que el servidor este corriendo");
        return 0;
    }

    Ishm = (InventarioShm *) shmat(shmID, NULL, 0);
    if (Ishm == (void *) -1) {
        perror("shmat");
        return 0;
    }

    //Usuarios

    shmID2 = shmget(keyShm_usr, sizeof(usuarioShm), PERMISOS);
    if (shmID2 == -1) {
        perror("shmget — asegurate de que el servidor este corriendo");
        return 0;
    }

    Ushm = (usuarioShm *) shmat(shmID2, NULL, 0);
    if (Ushm == (void *) -1) {
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
    if (Ishm&&Ushm){
      shmdt(Ishm);  
      shmdt(Ushm);
    } 
}

// ──────────────────────────────────────────
// 1. CARGAR CATALOGO DESDE SHM A LISTA
//    Sustituye: //insertar catalogo de memoria compartida a lista
// ──────────────────────────────────────────

void cargarCatalogo(listaarticulo cat) {

    if (!Ishm) {
        printf("DEBUG: shm es NULL\n");
        return;
    }
    printf("DEBUG: totalProductos = %d\n", Ishm->totalProductos);

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm->totalProductos; i++) {
        Producto *p = &Ishm->productos[i];

        if (!p->activo || p->existencias <= 0)
            continue;

        infoarticulo item;
        strncpy(item.producto, p->nombre, sizeof(item.producto) - 1);
        item.producto[sizeof(item.producto) - 1] = '\0';
        item.cantidad = p->existencias;
        item.precio   = p->precio;

        addarticulo(cat->NE, item, cat);
    }

    upSem(semID, SEM_INV);
}

// ──────────────────────────────────────────
// 2. ACTUALIZAR EXISTENCIAS EN SHM
//    Sustituye: //actualizar catalogo servidor
// ──────────────────────────────────────────

void actualizarExistencias(const char *nombreProducto, int cantidadVendida) {
    if (!Ishm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm->totalProductos; i++) {
        Producto *p = &Ishm->productos[i];

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
    if (!Ishm) return;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char fecha[20];
    strftime(fecha, sizeof(fecha), "%d/%m/%Y %H:%M", tm_info);


    downSem(semID, SEM_INV);

    if (Ishm->totalVentas < MAX_VENTAS) {
        Venta v;
        v.total = total;
        strncpy(v.usr,   usr,   sizeof(v.usr)   - 1);
        strncpy(v.fecha, fecha, sizeof(v.fecha) - 1);
        v.usr[sizeof(v.usr)     - 1] = '\0';
        v.fecha[sizeof(v.fecha) - 1] = '\0';

        Ishm->ventas[Ishm->totalVentas] = v;
        Ishm->totalVentas++;
    }

    upSem(semID, SEM_INV);

    FILE *f = fopen("ventas.txt", "a");
    if (f) {
        fprintf(f, "%s,%.2f,%s\n", usr, total, fecha);
        fclose(f);
    }

    // NOTIFICAR AL SERVIDOR PARA QUE GUARDE
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

void devolverExistencias(const char *nombreProducto, int cantidad) {
    if (!Ishm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm->totalProductos; i++) {
        Producto *p = &Ishm->productos[i];
        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->existencias += cantidad;
            break;
        }
    }

    upSem(semID, SEM_INV);

    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

int BuscarCorreo(char *correo){
    return 0;
}