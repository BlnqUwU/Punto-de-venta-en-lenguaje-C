#include "admin.h"
#include "listas.h"
#include "conexionipc.h"

#define ARCHIVO_ADMINS "admins.dat"

// ──────────────────────────────────────────
// LOGIN ADMIN — 5 CAMPOS
// ──────────────────────────────────────────

int buscarAdmin(char *usr) {

    lista admins = ObtenerUsuariosAdmin();
    for (int i = 0; i < admins->NE; i++) {
        info inf = get(i, admins);
        if (strcmp(inf.u.usr, usr) == 0) {
            liberarlista(&admins);
            return 1;
        }
    }
    liberarlista(&admins);
    return 0;

}

int registrarAdmin(usuario a) {

    return enviarusuarioAdminIPC(a, 0, NULL);
}

int solicitarSesionAdmin(usuario a) {
    return solicitarSesionAdmins(a);
}

/*void crearAdminSiNoExiste() {
    if (buscarAdmin("admin") == 0) {
        usuario a;
        char pass_plano[] = "admin";
        strcpy(a.nombre,   "Admin");
        strcpy(a.apellido, "Sistema");
        strcpy(a.correo,   "admin@sistema.com");
        strcpy(a.usr,      "admin");
        hash(pass_plano, a.pass);
        registrarAdmin(a);

    }
}*/

// ──────────────────────────────────────────
// SESION USUARIO
// ──────────────────────────────────────────

int SolicitarSesion(usuario a) {

    return solicitarSesionAdmin(a);
}

// ──────────────────────────────────────────
// UI
// ──────────────────────────────────────────
int checarservidor(){
    if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        int volver=1;
        return volver;
        
}


// ──────────────────────────────────────────
// ARCHIVOS DE USUARIOS
// ──────────────────────────────────────────

int RegistrarUsuario(usuario u, char *arch) {

    return enviarusuario(u, 0, NULL);
}

int ModificarAtributo(usuario u, char *usr) {

    return enviarusuario(u, 2, usr);
}

int ModificarAtributoCatalogo(articulo p, char *nombre) {

    return enviararticulo(p, 2, 0);
}

int BorrarUsuario(usuario a, char *usr){

    return enviarusuario(a, 3, NULL);
}

// ──────────────────────────────────────────
// OBTENER LISTAS
// ──────────────────────────────────────────

listaarticulo ObtenerProductos() {

    return ObtenerCatalogo();
}

int RegistrarProducto(articulo p, char *arch) {

    return enviararticulo(p, 0, 0);
}

int VerificarCorreo(char *correo) {
    int len = 0;
    int posicion_arroba = -1;
    int posicion_punto = -1;

    while (correo[len] != '\0') {
        if (correo[len] == '@') {
            posicion_arroba = len;
        } else if (correo[len] == '.') {
            posicion_punto = len;
        }
        len++;
    }
    
    if (posicion_arroba > 0 && posicion_punto > (posicion_arroba + 1) && posicion_punto < (len - 1)) {
        return 1; 
        // '@' debe estar seguido por al menos un carácter antes de '.' y '.' no puede ser el último carácter
    }

    return 0;
}

int BuscarCorreo(char *correo) {

    lista usuarios = ObtenerUsuarios();
    for (int i = 0; i < usuarios->NE; i++) {
        info inf = get(i, usuarios);
        if (strcmp(inf.u.correo, correo) == 0) {
            liberarlista(&usuarios);
            return 1;
        }
    }
    liberarlista(&usuarios);
    return 0;
}

int BuscarUsuario(char *usr) {

    lista usuarios = ObtenerUsuarios();
    for (int i = 0; i < usuarios->NE; i++) {
        info inf = get(i, usuarios);
        if (strcmp(inf.u.usr, usr) == 0) {
            liberarlista(&usuarios);
            return 1;
        }
    }
    liberarlista(&usuarios);
    return 0;

}

int BuscarProducto(char *producto) {

    listaarticulo cat = ObtenerCatalogo();
    for (int i = 0; i < cat->NE; i++) {
        articulo a = getarticulo(i, cat);
        if (strcmp(a.producto, producto) == 0) {
            liberarlistaarticulo(&cat);
            return 1;
        }
    }
    liberarlistaarticulo(&cat);
    return 0;
}

// CARGAR VENTAS POR RANGO DE DIAS
// dias=1 diario, dias=7 semanal, dias=30 mensual
// ──────────────────────────────────────────

void cargarVentasPorRango(listaventa lv, int dias) {

    int tipo = 0; // diario
    if (dias == 7)  tipo = 1; // semanal
    if (dias == 30) tipo = 2; // mensual

    listaventa ventas = obtenerVentas(tipo);
    // copiar ventas a lv
    for (int i = 0; i < ventas->NE; i++) {
        infoventa iv = getventa(i, ventas);
        addventa(lv->NE, iv, lv);
    }
    liberarlistaventa(&ventas);
}