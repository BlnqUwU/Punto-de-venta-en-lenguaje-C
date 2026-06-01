
#include "conexionipc.h"
#include "listas.h"
#include "memoria_compartida.h"
#include "servidor.h"

// ──────────────────────────────────────────
// CONEXION A IPC 
// (el servidor ya creo shm y semaforos)
// ──────────────────────────────────────────

static InventarioShm *Ishm   = NULL;
static listaarticulo carritoLocal = NULL;
static usuarioShm *Ushm   = NULL;
static ventaShm *Vshm   = NULL;
static ControlShm *Cshm   = NULL;
static carritoShm *Kshm   = NULL;
static int            semID = -1;
static int            shmID = -1;
static int            shmID2 = -1;
static int            shmID3 = -1;
static int            shmID4 = -1;
static int            shmID5 = -1;


int conectarServidor() {
    FILE *f = fopen(ARCHIVO_IPC, "a");
    if (f) fclose(f);
    key_t keyShm = ftok(ARCHIVO_IPC, 'M');
    key_t keyShm_usr = ftok(ARCHIVO_IPC, 'U');
    key_t keyShm_venta = ftok(ARCHIVO_IPC, 'V');
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
        //VENTAS
    shmID3 = shmget(keyShm_venta, sizeof(ventaShm),  PERMISOS);
    if (shmID3 == -1) {
        perror("shmget");
        exit(1);
    }

    Vshm = (ventaShm *) shmat(shmID3, NULL, 0);
    if (Vshm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // CONTROL

    key_t keyShm_ctrl = ftok(ARCHIVO_IPC, 'C');
    shmID4 = shmget(keyShm_ctrl, sizeof(ControlShm), PERMISOS);
    if (shmID4 == -1) {
        perror("shmget");
        return 0;
    }
    Cshm = (ControlShm *) shmat(shmID4, NULL, 0);
    if (Cshm == (void *) -1) {
        perror("shmat");
        return 0;
    }

    // CARRITO
    key_t keyShm_carrito = ftok(ARCHIVO_IPC, 'K');
    shmID5 = shmget(keyShm_carrito, sizeof(carritoShm), PERMISOS);
    if (shmID5 == -1) { perror("shmget carrito"); return 0; }
    Kshm = (carritoShm *) shmat(shmID5, NULL, 0);
    if (Kshm == (void *) -1) { perror("shmat carrito"); return 0; }


    // SEMAFOROS

    semID = semget(keySem, 5, PERMISOS);
    if (semID == -1) {
        perror("semget");
        return 0;
    }

    return 1;
}

void desconectarServidor() {
    if (Ishm) shmdt(Ishm);
    if (Ushm) shmdt(Ushm);
    if (Vshm) shmdt(Vshm);
    if (Cshm) shmdt(Cshm);
    if (Kshm) shmdt(Kshm);
}

// ──────────────────────────────────────────
// COMUNICACION CON SERVIDOR
// ──────────────────────────────────────────

//INDICE DE CRUD: 0=CREAR, 1=LEER, 2=ACTUALIZAR, 3=BORRAR

int enviararticulo(articulo p, int CRUD, int BD){
    //GUARDA ATRIBUTOS EN SHM DE INVENTARIO.
    //SI CRUD==1 DEBE ESPERAR RESPUESTA DE SERVIDOR Y RETORNAR LA VARIABLE REALIZADO
    // BD: 0 = Catalogo, 1 = Carrito

    if (BD == 1) {
        if (carritoLocal == NULL) crearlistaarticulo(&carritoLocal);

        if (CRUD == 0) {
            for (int i = 0; i < carritoLocal -> NE; i++) {
                articulo a = getarticulo(i, carritoLocal);

                if (strcmp(a.producto, p.producto) == 0) {
                    a.cantidad += p.cantidad;
                    setarticulo(i, a, carritoLocal);
                    return 1;
                }
            }
            addarticulo(carritoLocal -> NE, p, carritoLocal);
        } else if (CRUD == 2) {
            for (int i = 0; i < carritoLocal -> NE; i++) {
                articulo a = getarticulo(i, carritoLocal);
                if (strcmp(a.producto, p.producto) == 0) {
                    setarticulo(i, p, carritoLocal);
                    return 1;
                }
            }
        }else if (CRUD == 3) {
            for (int i = 0; i < carritoLocal -> NE; i++) {
                articulo a = getarticulo(i, carritoLocal);
                if (strcmp(a.producto, p.producto) == 0) {
                    borrararticulo(i, carritoLocal);
                    return 1;
                }
            }
        }
        return 1;
    }

    //BD = 0 - CATALOGO

    if (!Ishm) return 0;

    downSem(semID, SEM_INV);
    Ishm -> p = p;
    Ishm -> CRUD = CRUD;
    Ishm->BD        = BD;
    Ishm->realizado = 0;
    upSem(semID, SEM_INV);

    Cshm -> tipo = 0;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    return Ishm -> realizado;
}

int obtenerCantidadCatalogo(char *producto) {

    if (!Ishm) return 0;
    for (int i = 0; i < Ishm->totalCatalogo; i++)
        if (strcmp(Ishm->catalogo[i].producto, producto) == 0)
            return Ishm->catalogo[i].cantidad;
    return 0;
}

listaarticulo ObtenerCatalogo(){
    listaarticulo catalogo;
    crearlistaarticulo(&catalogo);
    if (!Ishm) return catalogo;

    downSem(semID, SEM_INV);
    Ishm -> CRUD = 1;
    Ishm -> BD = 0;
    Ishm -> realizado = 0;
    upSem(semID, SEM_INV);

    Cshm -> tipo = 0;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm -> totalCatalogo; i++) {
        if(Ishm->catalogo[i].cantidad>0)
        addarticulo(catalogo -> NE, Ishm -> catalogo[i], catalogo);
    }

    upSem(semID, SEM_INV);
    //GUARDA EL INVENTARIO DE LA MEMORIA COMPARIDA EN LA LISTA Y LA RETORNA
    return catalogo;
}


void limpiarCarrito() {
    if (carritoLocal != NULL) {
        Vaciarlistaarticulo(carritoLocal);
        free(carritoLocal);
        carritoLocal = NULL;
    }
}

listaarticulo ObtenerCarrito(){

    if (carritoLocal == NULL) crearlistaarticulo(&carritoLocal);
    return carritoLocal;
}

void guardarCarrito(char *usr) {
    if (!Kshm || carritoLocal == NULL) return;

    downSem(semID, SEM_INV);
    strncpy(Kshm->usr, usr, sizeof(Kshm->usr) - 1);
    Kshm->totalItems = 0;
    for (int i = 0; i < carritoLocal->NE && i < MAX_CARRITO; i++) {
        Kshm->items[i] = getarticulo(i, carritoLocal);
        Kshm->totalItems++;
    }
    Kshm->CRUD = 0; // guardar
    Kshm->realizado = 0;
    upSem(semID, SEM_INV);

    Cshm->tipo = 3;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

void cargarCarrito(char *usr) {

    if (!Kshm) return;

    downSem(semID, SEM_INV);
    strncpy(Kshm->usr, usr, sizeof(Kshm->usr) - 1);
    Kshm->CRUD = 1; // cargar
    Kshm->realizado = 0;
    upSem(semID, SEM_INV);

    Cshm->tipo = 3;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    // llenar carritoLocal con lo que respondio el servidor
    if (carritoLocal == NULL) crearlistaarticulo(&carritoLocal);
    else Vaciarlistaarticulo(carritoLocal);

    downSem(semID, SEM_INV);
    for (int i = 0; i < Kshm->totalItems; i++) {
        addarticulo(carritoLocal->NE, Kshm->items[i], carritoLocal);
    }
    upSem(semID, SEM_INV);
}

int enviarusuario(usuario u, int CRUD, char *nombreUsuario){
    //GUARDA ATRIBUTOS EN SHM DE USUARIO

    if (!Ushm) return 0;

    //SI CRUD==1 DEBE ESPERAR RESPUESTA DE SERVIDOR Y RETORNAR LA VARIABLE
    //REALIZADO

    downSem(semID, SEM_USR);
    Ushm->u         = u;
    Ushm->CRUD      = CRUD;
    Ushm->realizado = 0;
    Ushm -> BD      = 0;
    if (nombreUsuario != NULL) {
        strncpy(Ushm -> usr_original, nombreUsuario, sizeof(Ushm -> usr_original) - 1);
    } else {
        Ushm -> usr_original[0] = '\0';
    }
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    return Ushm -> realizado;
}

int enviarusuarioAdminIPC(usuario u, int CRUD, char *nombreUsuario){
    //GUARDA ATRIBUTOS EN SHM DE Admin

    if (!Ushm) return 0;

    //SI CRUD==1 DEBE ESPERAR RESPUESTA DE SERVIDOR Y RETORNAR LA VARIABLE
    //REALIZADO

    downSem(semID, SEM_USR);
    Ushm->u         = u;
    Ushm->CRUD      = CRUD;
    Ushm->realizado = 0;
    Ushm -> BD      = 1;
    if (nombreUsuario != NULL) {
        strncpy(Ushm -> usr_original, nombreUsuario, sizeof(Ushm -> usr_original) - 1);
    } else {
        Ushm -> usr_original[0] = '\0';
    }
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    return Ushm -> realizado;
}

int solicitarSesion(usuario u){
    //GUARDA USUARIO Y CRUD==1 EN SHM DE USUARIO

    if (!Ushm) return 0;

    //ESPERA RESPUESTA DEL SERVIDOR, RETORNA LA VARIABLE REALIZADO

    downSem(semID, SEM_USR);
    Ushm -> u = u;
    Ushm -> CRUD = 1;
    Ushm -> realizado = 0;
    Ushm -> BD      = 0;
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    return Ushm -> realizado;
}

int solicitarSesionAdmins(usuario u){
    //GUARDA USUARIO Y CRUD==1 EN SHM DE USUARIO

    if (!Ushm) return 0;

    //ESPERA RESPUESTA DEL SERVIDOR, RETORNA LA VARIABLE REALIZADO

    downSem(semID, SEM_USR);
    Ushm -> u = u;
    Ushm -> CRUD = 1;
    Ushm -> realizado = 0;
    Ushm -> BD = 1;
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    return Ushm -> realizado;
}

usuario obtenerUsuario(usuario u){

    usuario vacio = {"","","","",""};
    if (!Ushm) return vacio;

    //GUARDA ATRIBUTOS EN SHM Y CRUD==1 DE USUARIO
    //ESPERA RESPUESTA DEL SERVIDOR Y RETORNA EL USUARIO RECIBIDO DEL SERVIDOR

    downSem(semID, SEM_USR);
    Ushm -> u = u;
    Ushm -> CRUD = 1;
    Ushm -> realizado = 0;
    Ushm -> BD = 0;
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    if (Ushm -> realizado == 1) return Ushm -> u;
    return vacio;
}

usuario obtenerUsuarioAdmin(usuario u){

    usuario vacio = {"","","","",""};
    if (!Ushm) return vacio;

    //GUARDA ATRIBUTOS EN SHM Y CRUD==1 DE USUARIO
    //ESPERA RESPUESTA DEL SERVIDOR Y RETORNA EL USUARIO RECIBIDO DEL SERVIDOR

    downSem(semID, SEM_USR);
    Ushm -> u = u;
    Ushm -> CRUD = 1;
    Ushm -> realizado = 0;
    Ushm -> BD = 1;
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    if (Ushm -> realizado == 1) return Ushm -> u;
    return vacio;
}

lista ObtenerUsuarios(){
    lista usuarios;
    crearlista(&usuarios);
    if (!Ushm) return usuarios;

    //GUARDA LOS USUARIOS DE LA MEMORIA COMPARTIDA Y CRUD==1 Y LA RETORNA

    downSem(semID, SEM_USR);
    Ushm->CRUD = 1;
    Ushm -> realizado = 0;
    Ushm -> BD = 0;
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    downSem(semID, SEM_USR);
    for (int i = 0; i < Ushm -> totalUsuarios; i++) {
        info inf;
        inf.u = Ushm -> usuarios[i];
        add(usuarios -> NE, inf, usuarios);
    }

    upSem(semID, SEM_USR);

    return usuarios;
}

lista ObtenerUsuariosAdmin(){
    lista usuarios;
    crearlista(&usuarios);
    if (!Ushm) return usuarios;

    //GUARDA LOS USUARIOS DE LA MEMORIA COMPARTIDA Y CRUD==1 Y LA RETORNA

    downSem(semID, SEM_USR);
    Ushm->CRUD = 1;
    Ushm -> realizado = 0;
    Ushm -> BD = 1;
    upSem(semID, SEM_USR);

    Cshm -> tipo = 1;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    downSem(semID, SEM_USR);
    for (int i = 0; i < Ushm -> totalUsuarios; i++) {
        info inf;
        inf.u = Ushm -> usuarios[i];
        add(usuarios -> NE, inf, usuarios);
    }

    upSem(semID, SEM_USR);

    return usuarios;
}

listaventa obtenerVentas(int tipo){

    // tipo: 0=diario, 1=semanal, 3=mensual
    listaventa ventas;
    crearlistaventa(&ventas);
    if (!Vshm) return ventas;

    downSem(semID, SEM_VTA);
    Vshm -> tipo = tipo;
    Vshm -> CRUD = 1;
    Vshm -> realizado = 0;
    upSem(semID, SEM_VTA);

    Cshm -> tipo = 2;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

    downSem(semID, SEM_VTA);
    for (int i = 0; i < Vshm -> totalVentas; i++) {
        infoventa iv;
        iv.v = Vshm -> ventas[i];
        addventa(ventas -> NE, iv, ventas);
    }
    upSem(semID, SEM_VTA);

    return ventas;
}

void enviarVenta(venta v){

    if (!Vshm) return;

    downSem(semID, SEM_VTA);
    Vshm -> v = v;
    Vshm -> CRUD = 0;
    Vshm -> realizado = 0;
    upSem(semID, SEM_VTA);

    Cshm -> tipo = 2;
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);

}