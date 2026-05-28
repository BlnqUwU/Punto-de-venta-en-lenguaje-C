
#include "conexionipc.h"
#include "listas.h"
#include "memoria_compartida.h"

// ──────────────────────────────────────────
// CONEXION A IPC 
// (el servidor ya creo shm y semaforos)
// ──────────────────────────────────────────

static InventarioShm *Ishm   = NULL;
static usuarioShm *Ushm   = NULL;
static ventaShm *Vshm   = NULL;
static int            semID = -1;
static int            shmID = -1;
static int            shmID2 = -1;
static int            shmID3 = -1;


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
    int shmID3 = shmget(keyShm_venta, sizeof(ventaShm), IPC_CREAT | PERMISOS);
    if (shmID3 == -1) {
        perror("shmget");
        exit(1);
    }

    ventaShm *Vshm = (ventaShm *) shmat(shmID3, NULL, 0);
    if (Vshm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // SEMAFOROS

    semID = semget(keySem, 3, PERMISOS);
    if (semID == -1) {
        perror("semget");
        return 0;
    }

    return 1;
}

void desconectarServidor() {
    if (Ishm&&Ushm&&Vshm){
      shmdt(Ishm);  
      shmdt(Ushm);
      shmdt(Vshm);
    } 
}

// ──────────────────────────────────────────
// COMUNICACION CON SERVIDOR
// ──────────────────────────────────────────

//INDICE DE CRUD: 0=CREAR, 1=LEER, 2=ACTUALIZAR, 3=BORRAR

int enviararticulo(articulo p, int CRUD, int BD){
    //GUARDA ATRIBUTOS EN SHM DE INVENTARIO.

    //SI CRUD==1 DEBE ESPERAR RESPUESTA DE SERVIDOR Y RETORNAR LA VARIABLE
    //REALIZADO 
    return 0;
}

listaarticulo ObtenerCatalogo(){
    listaarticulo catalogo;

    //GUARDA EL INVENTARIO DE LA MEMORIA COMPARIDA EN LA LISTA Y LA RETORNA

    return catalogo;
}

listaarticulo ObtenerCarrito(){
    listaarticulo carrito;

    //GUARDA EL INVENTARIO DE LA MEMORIA COMPARIDA EN LA LISTA Y LA RETORNA

    return carrito;
}

int enviarusuario(usuario u, int CRUD){
    //GUARDA ATRIBUTOS EN SHM DE USUARIO

    //SI CRUD==1 DEBE ESPERAR RESPUESTA DE SERVIDOR Y RETORNAR LA VARIABLE
    //REALIZADO
    
    return 0;

}

int solicitarSesion(usuario u){
    //GUARDA USUARIO Y CRUD==1 EN SHM DE USUARIO

    //ESPERA RESPUESTA DEL SERVIDOR, RETORNA LA VARIABLE REALIZADO
    return 0;
}

usuario obtenerUsuario(usuario u){
    usuario usuario;
    //GUARDA ATRIBUTOS EN SHM Y CRUD==1 DE USUARIO
    //ESPERA RESPUESTA DEL SERVIDOR Y RETORNA EL USUARIO RECIBIDO DEL SERVIDOR
    return usuario;
}

lista ObtenerUsuarios(){
    lista usuarios;
    //GUARDA LOS USUARIOS DE LA MEMORIA COMPARTIDA Y CRUD==1 Y LA RETORNA
    return usuarios;
}

listaventa obtenerVentas(int tipo){
    listaventa ventas;

    //ENVIA AL SERVIDOR EL ARGUMENTO Y RECIBE UNA LISTA CON LAS VENTAS SEGUN EL TIPO

    return ventas;
}

void enviarVenta(venta v){
    //GUARDA LA VENTA EN LA MEMORIA COMPARTIDA Y LA MANDA AL SERVIDOR
    return;
}