#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include "conexionipc.h"

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
// CONEXION A SERVIDOR
// (el servidor ya creo shm y semaforos)
// ──────────────────────────────────────────