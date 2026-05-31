#include "memoria_compartida.h"
#include "servidor.h"
#include <signal.h>

//GLOBALES

int shmID;
int shmID2;
int shmID3;
int shmID4;
int semID;

// ──────────────────────────────────────────
// ARGUMENTOS PARA HILO DE CLIENTE
// ──────────────────────────────────────────

typedef struct {
    int semID;
    InventarioShm *Ishm;
    usuarioShm *Ushm;
    ventaShm *Vshm;
    ControlShm *Cshm;
} ArgsHilo;

// ──────────────────────────────────────────
// HILO QUE ATIENDE UNA PETICION
// ──────────────────────────────────────────

void *atenderPeticion(void *arg) {
    ArgsHilo *args = (ArgsHilo *) arg;

    int tipo = args -> Cshm -> tipo;
    printf("[SERVIDOR] Peticion recibida. Tipo: %d. Atendiendo...\n", tipo);

    if (tipo == 0) {
        // INVENTARIO
        CRUDcatalogo(args -> Ishm, args->Ishm->CRUD, 0);
    } else if (tipo == 1) {
        //USUARIOS
        CRUDusuario(args->Ushm, args->Ushm->CRUD);
    } else if (tipo == 2) {
        // VENTAS
        CRUDventas(args -> Vshm, args->Vshm->CRUD);
    }

    upSem(args->semID, SEM_ACK);
    return NULL;

}

void limpiar(int sig) {
    shmctl(shmID,  IPC_RMID, 0);
    shmctl(shmID2, IPC_RMID, 0);
    shmctl(shmID3, IPC_RMID, 0);
    shmctl(shmID4, IPC_RMID, 0);
    semctl(semID, 0, IPC_RMID);
    exit(0);
}

// ──────────────────────────────────────────
// MAIN
// ──────────────────────────────────────────

int main() {

    // CREAR ARCHIVO BASE PARA FTOK SI NO EXISTE
    FILE *f = fopen(ARCHIVO_IPC, "a");
    if (f) fclose(f);

    // GENERAR LLAVES IPC
    key_t keyShm = ftok(ARCHIVO_IPC, 'M');
    key_t keyShm_usr = ftok(ARCHIVO_IPC, 'U');
    key_t keyShm_venta = ftok(ARCHIVO_IPC, 'V');
    key_t keyShm_ctrl = ftok(ARCHIVO_IPC, 'C');

    key_t keySem = ftok(ARCHIVO_IPC, 'S');
    if (keyShm == -1 || keySem == -1) {
        perror("ftok");
        exit(1);
    }

    // CREAR MEMORIA COMPARTIDA
        //INVENTARIO
    shmID = shmget(keyShm, sizeof(InventarioShm), IPC_CREAT | PERMISOS);
    if (shmID == -1) {
        perror("shmget");
        exit(1);
    }

    InventarioShm *Ishm = (InventarioShm *) shmat(shmID, NULL, 0);
    if (Ishm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

        //USUARIOS
    shmID2 = shmget(keyShm_usr, sizeof(usuarioShm), IPC_CREAT | PERMISOS);
    if (shmID2 == -1) {
        perror("shmget");
        exit(1);
    }

    usuarioShm *Ushm = (usuarioShm *) shmat(shmID2, NULL, 0);
    if (Ushm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

        //VENTAS
    shmID3 = shmget(keyShm_venta, sizeof(ventaShm), IPC_CREAT | PERMISOS);
    if (shmID3 == -1) {
        perror("shmget");
        exit(1);
    }

    ventaShm *Vshm = (ventaShm *) shmat(shmID3, NULL, 0);
    if (Vshm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // CONTROL

    shmID4 = shmget(keyShm_ctrl, sizeof(ControlShm), IPC_CREAT | PERMISOS);
    if (shmID4 == -1) {
        perror("shmget");
        exit(1);
    }

    ControlShm *Cshm = (ControlShm *) shmat(shmID4, NULL, 0);
    if (Cshm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // CARGAR INVENTARIO DESDE ARCHIVO (si existe)

    CRUDcatalogo(Ishm, 1, 0);
    if (Ishm->totalCatalogo > 0)
        printf("[SERVIDOR] Catalogo cargado: %d productos.\n", Ishm->totalCatalogo);
    else
        printf("[SERVIDOR] Catalogo vacio o archivo no encontrado.\n");

    //CARGAR USUARIOS

    FILE *fUsr = fopen(ARCHIVO_USR, "r");
    if (fUsr) {
        char lineaUsr[300];
        Ushm->totalUsuarios = 0;
        while (fgets(lineaUsr, sizeof(lineaUsr), fUsr) && Ushm->totalUsuarios < MAX_USUARIOS) {
            usuario u;
            sscanf(lineaUsr, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
                   u.nombre, u.apellido, u.correo, u.usr, u.pass);
            Ushm->usuarios[Ushm->totalUsuarios] = u;
            Ushm->totalUsuarios++;
        }
        fclose(fUsr);
        printf("[SERVIDOR] Usuarios cargados: %d.\n", Ushm->totalUsuarios);
    } else {
        printf("[SERVIDOR] Sin usuarios previos.\n");
    }


    // CREAR SEMAFOROS
    semID = semget(keySem, 5, IPC_CREAT | PERMISOS);
    if (semID == -1) {
        perror("semget");
        exit(1);
    }

    // INICIALIZAR SEMAFOROS
    // SEM_INV  = 1 -> inventario libre
    // SEM_USR  = 1 -> usuarios libre
    // SEM_VTA  = 1 -> ventas libre
    // SEM_REQ  = 0 -> sin peticiones //peticiones de usuarios
    // SEM_ACK  = 0 -> sin respuestas   //avisa si servidor esta libre

    union semun arg;
    arg.val = 1; semctl(semID, SEM_INV, SETVAL, arg);
    arg.val = 1; semctl(semID, SEM_USR, SETVAL, arg);
    arg.val = 1; semctl(semID, SEM_VTA, SETVAL, arg);
    arg.val = 0; semctl(semID, SEM_REQ, SETVAL, arg);
    arg.val = 0; semctl(semID, SEM_ACK, SETVAL, arg);


    printf("[SERVIDOR] PID: %d listo. Esperando clientes...\n\n", getpid());

    signal(SIGINT, limpiar);
    signal(SIGTERM, limpiar);

    // BUCLE PRINCIPAL — ATIENDE PETICIONES
    while (1) {

        // ESPERAR PETICION DE ALGUN CLIENTE
        downSem(semID, SEM_REQ);

        // CREAR HILO PARA ATENDER
        pthread_t hilo;
        ArgsHilo args = {semID, Ishm, Ushm, Vshm, Cshm};

        if (pthread_create(&hilo, NULL, atenderPeticion, &args) != 0) {
            perror("pthread_create");
            upSem(semID, SEM_ACK);
        } else {
            pthread_join(hilo, NULL);
        }

        // GUARDAR INVENTARIO ACTUALIZADO EN ARCHIVO
        /*downSem(semID, SEM_INV);
        guardarInventario(Ishm);
        upSem(semID, SEM_INV);*/
    }

    // LIMPIEZA (no se llega aqui en condiciones normales)
    shmdt(Ishm);
    shmdt(Ushm);
    shmdt(Vshm);
    shmdt(Cshm);
    shmctl(shmID, IPC_RMID, 0);
    shmctl(shmID2, IPC_RMID, 0);
    semctl(semID, 0, IPC_RMID);

    return 0;
}