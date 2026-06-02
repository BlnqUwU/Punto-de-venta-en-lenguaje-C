#include "memoria_compartida.h"
#include "servidor.h"
#include <signal.h>

//GLOBALES

int shmID;
int shmID2;
int shmID3;
int shmID4;
int shmID5;
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
    carritoShm *Kshm;
} ArgsHilo;

// ──────────────────────────────────────────
// HILO QUE ATIENDE UNA PETICION
// ──────────────────────────────────────────

void *atenderPeticion(void *arg) {
    ArgsHilo *args = (ArgsHilo *) arg;

    int tipo = args -> Cshm -> tipo;
    printf("[SERVIDOR] Peticion tipo=%d | PID cliente=%d | Hilo=%lu\n",
           tipo, args->Cshm->pid_cliente, (unsigned long)pthread_self());

    if (tipo == 0) {
        // INVENTARIO
        CRUDcatalogo(args -> Ishm, args->Ishm->CRUD, 0);
    } else if (tipo == 1) {
        //USUARIOS

        usuarioShm *UshPriv = NULL;
        int shmPrivID = shmget(args->Cshm->key_privada, sizeof(usuarioShm), PERMISOS);
        if (shmPrivID != -1) {
            UshPriv = (usuarioShm *) shmat(shmPrivID, NULL, 0);
            if (UshPriv == (void *) -1) UshPriv = NULL;
        }
        if (UshPriv) {
            // copiar totales del Ushm global para que el CRUD pueda buscar
            UshPriv->totalUsuarios = args->Ushm->totalUsuarios;
            UshPriv->totalAdmins   = args->Ushm->totalAdmins;
            memcpy(UshPriv->usuarios, args->Ushm->usuarios, sizeof(usuario) * args->Ushm->totalUsuarios);
            memcpy(UshPriv->admins,   args->Ushm->admins,   sizeof(usuario) * args->Ushm->totalAdmins);
            CRUDusuario(UshPriv, UshPriv->CRUD);
            // si hubo cambios en el arreglo, propagar al Ushm global
            if (UshPriv->CRUD == 0 || UshPriv->CRUD == 2 || UshPriv->CRUD == 3) {
                args->Ushm->totalUsuarios = UshPriv->totalUsuarios;
                args->Ushm->totalAdmins   = UshPriv->totalAdmins;
                memcpy(args->Ushm->usuarios, UshPriv->usuarios, sizeof(usuario) * UshPriv->totalUsuarios);
                memcpy(args->Ushm->admins,   UshPriv->admins,   sizeof(usuario) * UshPriv->totalAdmins);
            }
            shmdt(UshPriv);
        } else {
            // shm publica si no se pudo conectar a la privada
            CRUDusuario(args->Ushm, args->Ushm->CRUD);
        }

    } else if (tipo == 2) {
        // VENTAS
        CRUDventas(args -> Vshm, args->Vshm->CRUD);
    } else if (tipo == 3) {
        CRUDcarrito(args->Kshm, args->Kshm->CRUD);
    }

    upSem(args->semID, SEM_ACK);
    return NULL;


    // LEER PETICION DESDE MEMORIA COMPARTIDA

    // POR AHORA SOLO CONFIRMA RECEPCION
    // AQUI SE EXPANDIRA CON LOGICA DE VENTA
}

void limpiar(int sig) {
    shmctl(shmID,  IPC_RMID, 0);
    shmctl(shmID2, IPC_RMID, 0);
    shmctl(shmID3, IPC_RMID, 0);
    shmctl(shmID4, IPC_RMID, 0);
    shmctl(shmID5, IPC_RMID, 0);
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
    key_t keyShm_carrito = ftok(ARCHIVO_IPC, 'K');
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

    //CARRITO
    shmID5 = shmget(keyShm_carrito, sizeof(carritoShm), IPC_CREAT | PERMISOS);
    if (shmID5 == -1) {
        perror("shmget carrito");
        exit(1);
    }

    carritoShm *Kshm = (carritoShm *) shmat(shmID5, NULL, 0);
    if (Kshm == (void *) -1) {
        perror("shmat carrito");
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

    // cargar admins a Ushm->admins[]
    FILE *fAdm = fopen("admins.dat", "r");
    if (fAdm) {
        char lineaAdm[300];
        Ushm->totalAdmins = 0;
        while (fgets(lineaAdm, sizeof(lineaAdm), fAdm) && Ushm->totalAdmins < MAX_ADMINS) {
            usuario a;
            sscanf(lineaAdm, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
                   a.nombre, a.apellido, a.correo, a.usr, a.pass);
            Ushm->admins[Ushm->totalAdmins] = a;
            Ushm->totalAdmins++;
        }
        fclose(fAdm);
        printf("[SERVIDOR] Admins cargados: %d.\n", Ushm->totalAdmins);
    } else {
        // crear admin por defecto con hash de "admin"
        usuario a;
        char pass_plano[] = "admin";
        strcpy(a.nombre, "Admin");
        strcpy(a.apellido, "Sistema");
        strcpy(a.correo, "admin@sistema.com");
        strcpy(a.usr, "admin");
        hash(pass_plano, a.pass);
        Ushm->admins[0] = a;
        Ushm->totalAdmins = 1;
        FILE *fNew = fopen("admins.dat", "w");
        if (fNew) {
            fprintf(fNew, "%s,%s,%s,%s,%s,\n", a.nombre, a.apellido, a.correo, a.usr, a.pass);
            fclose(fNew);
        }
        printf("[SERVIDOR] Admin por defecto creado. usr: admin | pass: admin\n");
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
    // SEM_REQ  = 0 -> sin peticiones
    // SEM_ACK  = 0 -> sin respuestas

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
        ArgsHilo args = {semID, Ishm, Ushm, Vshm, Cshm, Kshm};

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
    shmdt(Kshm);
    shmctl(shmID, IPC_RMID, 0);
    shmctl(shmID2, IPC_RMID, 0);
    shmctl(shmID5, IPC_RMID, 0);
    semctl(semID, 0, IPC_RMID);


    return 0;
}