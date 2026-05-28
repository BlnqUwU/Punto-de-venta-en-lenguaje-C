#include "servidor.h"

int BuscarAtributo(char *atributo, char *BD){
    return 0;
}

int EnviarSesion(usuario u, char *BD){
    return 0;
}
listaarticulo EnviarCatalogo(){
    listaarticulo catalogo;

    return catalogo;
}
lista EnviarUsuarios(){
    lista usuarios;

    return usuarios;
}


void CRUDusuario(lista usuarios, int CRUD){
    return;
}

void CRUDcatalogo(listaarticulo catalogo, int CRUD, int BD){
    return;
}

void CRUDventas(listaventa ventas, int CRUD){
    return;
}


// ──────────────────────────────────────────
// ENCRIPTACION XOR
// ──────────────────────────────────────────

/*void encriptar(const char *entrada, const char *salida) {
    FILE *fin  = fopen(entrada, "rb");
    FILE *fout = fopen(salida,  "wb");

    if (!fin || !fout) {
        perror("encriptar: fopen");
        if (fin)  fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    int c;
    while ((c = fgetc(fin)) != EOF)
        fputc(c ^ XOR_KEY, fout);

    fclose(fin);
    fclose(fout);
}

void desencriptar(const char *entrada, const char *salida) {
    // XOR es simetrico, misma operacion
    encriptar(entrada, salida);
}*/

// ──────────────────────────────────────────
// ARCHIVO DE INVENTARIO
// ──────────────────────────────────────────

int guardarInventario(InventarioShm *shm) {
    // GUARDAR EN ARCHIVO TEMPORAL SIN ENCRIPTAR
    /*FILE *f = fopen("inventario.tmp", "wb");
    if (!f) {
        perror("guardarInventario: fopen");
        return 0;
    }

    fwrite(shm, sizeof(InventarioShm), 1, f);
    fclose(f);

    // ENCRIPTAR Y GUARDAR COMO ARCHIVO FINAL
    encriptar("inventario.tmp", ARCHIVO_INV);
    remove("inventario.tmp");
*/
    return 1;
}

int cargarInventario(InventarioShm *shm) {
    /*FILE *f = fopen(ARCHIVO_INV, "rb");
    if (!f) {
        // NO EXISTE EL ARCHIVO, INVENTARIO VACIO
        shm->totalProductos = 0;
        shm->totalVentas    = 0;
        return 0;
    }
    fclose(f);

    // DESENCRIPTAR A ARCHIVO TEMPORAL
    desencriptar(ARCHIVO_INV, "inventario.tmp");

    f = fopen("inventario.tmp", "rb");
    if (!f) {
        perror("cargarInventario: fopen tmp");
        return 0;
    }

    fread(shm, sizeof(InventarioShm), 1, f);
    fclose(f);
    remove("inventario.tmp");
*/
    return 1;
}

// ──────────────────────────────────────────
// CRUD INVENTARIO
// ──────────────────────────────────────────

int agregarProducto(InventarioShm *shm, articulo p) {
   /* if (shm->totalProductos >= MAX_PRODUCTOS)
        return 0;   // inventario lleno

    // GENERAR ID UNICO (el mayor id existente + 1)
    int maxId = 0;
    for (int i = 0; i < shm->totalProductos; i++) {
        if (shm->productos[i].activo && shm->productos[i].id > maxId)
            maxId = shm->productos[i].id;
    }

    p.id     = maxId + 1;
    p.activo = 1;

    shm->productos[shm->totalProductos] = p;
    shm->totalProductos++;
*/
    return 1;
}

int buscarProducto(InventarioShm *shm, int id) {
    /*for (int i = 0; i < shm->totalProductos; i++) {
        if (shm->productos[i].activo && shm->productos[i].id == id)
            return i;   // retorna el indice en el arreglo
    }*/
    return -1;  // no encontrado
}

int eliminarProducto(InventarioShm *shm, int id) {
    /*int idx = buscarProducto(shm, id);
    if (idx == -1)
        return 0;   // no existe

    shm->productos[idx].activo = 0;*/
    return 1;
}

int modificarProducto(InventarioShm *shm, int id, articulo nuevo) {
    /*int idx = buscarProducto(shm, id);
    if (idx == -1)
        return 0;

    // CONSERVAR ID Y ESTADO ACTIVO
    nuevo.id     = id;
    nuevo.activo = 1;

    shm->productos[idx] = nuevo;*/
    return 1;
}

int venderProducto(InventarioShm *shm, int id, int cantidad, const char *usr) {
    /*int idx = buscarProducto(shm, id);
    if (idx == -1)
        return 0;   // producto no existe

    if (shm->productos[idx].existencias < cantidad)
        return -1;  // sin suficientes existencias

    // DESCONTAR EXISTENCIAS
    shm->productos[idx].existencias -= cantidad;

    // REGISTRAR VENTA
    if (shm->totalVentas < MAX_VENTAS) {
        Venta v;
        v.total = shm->productos[idx].precio * cantidad;
        strncpy(v.usr, usr, sizeof(v.usr) - 1);
        v.usr[sizeof(v.usr) - 1] = '\0';

        shm->ventas[shm->totalVentas] = v;
        shm->totalVentas++;
    }*/

    return 1;
}


// ──────────────────────────────────────────
// ARCHIVO DE USUARIOS
// ──────────────────────────────────────────

int guardarUsuarios(usuarioShm *shm) {
    // GUARDAR EN ARCHIVO TEMPORAL SIN ENCRIPTAR
    /*FILE *f = fopen("usuarios.tmp", "wb");
    if (!f) {
        perror("guardarUsuarios: fopen");
        return 0;
    }

    fwrite(shm, sizeof(usuarioShm), 1, f);
    fclose(f);

    // ENCRIPTAR Y GUARDAR COMO ARCHIVO FINAL
    encriptar("usuarios.tmp", ARCHIVO_INV);
    remove("usuarios.tmp");
*/
    return 1;
}

int cargarUsuarios(InventarioShm *shm) {
    /*FILE *f = fopen(ARCHIVO_INV, "rb");
    if (!f) {
        // NO EXISTE EL ARCHIVO, INVENTARIO VACIO
        shm->totalProductos = 0;
        shm->totalVentas    = 0;
        return 0;
    }
    fclose(f);

    // DESENCRIPTAR A ARCHIVO TEMPORAL
    desencriptar(ARCHIVO_INV, "usuarios.tmp");

    f = fopen("usuarios.tmp", "rb");
    if (!f) {
        perror("cargarUsuarios: fopen tmp");
        return 0;
    }

    fread(shm, sizeof(usuarioShm), 1, f);
    fclose(f);
    remove("usuarios.tmp");*/

    return 1;
}