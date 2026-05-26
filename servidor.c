#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "inventario.h"
#include "admin.h"

// ──────────────────────────────────────────
// ARGUMENTOS PARA HILO DE CLIENTE
// ──────────────────────────────────────────

typedef struct {
    int semID;
    InventarioShm *shm;
} ArgsHilo;

// ──────────────────────────────────────────
// HILO QUE ATIENDE UNA PETICION
// ──────────────────────────────────────────

void *atenderPeticion(void *arg) {
    ArgsHilo *args = (ArgsHilo *) arg;

    // LEER PETICION DESDE MEMORIA COMPARTIDA

    // POR AHORA SOLO CONFIRMA RECEPCION
    // AQUI SE EXPANDIRA CON LOGICA DE VENTA

    printf("[SERVIDOR] Peticion recibida. Atendiendo...\n");

    upSem(args->semID, SEM_ACK);

    return NULL;
}

// ──────────────────────────────────────────
// MAIN
// ──────────────────────────────────────────

int main() {

    // CREAR ARCHIVO BASE PARA FTOK SI NO EXISTE
    FILE *f = fopen(ARCHIVO_IPC, "a");
    if (f) fclose(f);

    // CREAR ADMIN POR DEFECTO SI NO EXISTE
    crearAdminSiNoExiste();

    // GENERAR LLAVES IPC
    key_t keyShm = ftok(ARCHIVO_IPC, 'M');
    key_t keySem = ftok(ARCHIVO_IPC, 'S');
    if (keyShm == -1 || keySem == -1) {
        perror("ftok");
        exit(1);
    }

    // CREAR MEMORIA COMPARTIDA
    int shmID = shmget(keyShm, sizeof(InventarioShm), IPC_CREAT | PERMISOS);
    if (shmID == -1) {
        perror("shmget");
        exit(1);
    }

    InventarioShm *shm = (InventarioShm *) shmat(shmID, NULL, 0);
    if (shm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // CARGAR INVENTARIO DESDE ARCHIVO (si existe)
    if (cargarInventario(shm))
        printf("[SERVIDOR] Inventario cargado: %d productos.\n", shm->totalProductos);
    else
        printf("[SERVIDOR] Inventario nuevo.\n");

    // CREAR SEMAFOROS
    int semID = semget(keySem, 3, IPC_CREAT | PERMISOS);
    if (semID == -1) {
        perror("semget");
        exit(1);
    }

    // INICIALIZAR SEMAFOROS
    // SEM_INV  = 1 -> inventario libre
    // SEM_REQ  = 0 -> sin peticiones
    // SEM_ACK  = 0 -> sin respuestas

    union semun arg;
    arg.val = 1; semctl(semID, SEM_INV, SETVAL, arg);
    arg.val = 0; semctl(semID, SEM_REQ, SETVAL, arg);
    arg.val = 0; semctl(semID, SEM_ACK, SETVAL, arg);

    printf("[SERVIDOR] PID: %d listo. Esperando clientes...\n\n", getpid());

    // BUCLE PRINCIPAL — ATIENDE PETICIONES
    while (1) {

        // ESPERAR PETICION DE ALGUN CLIENTE
        downSem(semID, SEM_REQ);

        // CREAR HILO PARA ATENDER
        pthread_t hilo;
        ArgsHilo args = {semID, shm};

        if (pthread_create(&hilo, NULL, atenderPeticion, &args) != 0) {
            perror("pthread_create");
            upSem(semID, SEM_ACK);
        } else {
            pthread_join(hilo, NULL);
        }

        // GUARDAR INVENTARIO ACTUALIZADO EN ARCHIVO
        downSem(semID, SEM_INV);
        guardarInventario(shm);
        upSem(semID, SEM_INV);
    }

    // LIMPIEZA (no se llega aqui en condiciones normales)
    shmdt(shm);
    shmctl(shmID, IPC_RMID, 0);
    semctl(semID, 0, IPC_RMID);

    return 0;
}