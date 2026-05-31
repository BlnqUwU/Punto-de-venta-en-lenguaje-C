#include "admin.h"
#include "listas.h"
#include "conexionipc.h"

#define ARCHIVO_ADMINS "admins.txt"

// ──────────────────────────────────────────
// LOGIN ADMIN — 5 CAMPOS
// ──────────────────────────────────────────

int buscarAdmin(char *usr) {
    char linea[300];
    FILE *archivo = fopen(ARCHIVO_ADMINS, "r");
    if (!archivo) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[100], apellido[100], correo[100], u[100], pass[100];
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               nombre, apellido, correo, u, pass);
        if (strcmp(u, usr) == 0) {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}

int registrarAdmin(usuario a) {
    FILE *archivo = fopen(ARCHIVO_ADMINS, "a");
    if (!archivo) {
        perror("registrarAdmin: fopen");
        return 0;
    }
    fprintf(archivo, "%s,%s,%s,%s,%s,\n",
            a.nombre, a.apellido, a.correo, a.usr, a.pass);
    fclose(archivo);
    return 1;
}

int solicitarSesionAdmin(usuario a) {
    char linea[300];
    FILE *archivo = fopen(ARCHIVO_ADMINS, "r");
    if (!archivo) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[100], apellido[100], correo[100], usr[100], pass[100];
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               nombre, apellido, correo, usr, pass);
        if (strcmp(usr, a.usr) == 0 && strcmp(pass, a.pass) == 0) {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}

void crearAdminSiNoExiste() {
    if (buscarAdmin("admin") == 0) {
        usuario a;
        char pass_plano[] = "admin";
        strcpy(a.nombre,   "Admin");
        strcpy(a.apellido, "Sistema");
        strcpy(a.correo,   "admin@sistema.com");
        strcpy(a.usr,      "admin");
        hash(pass_plano, a.pass);
        registrarAdmin(a);
        printf("[SERVIDOR] Admin creado. usr: admin | pass: admin\n");
    }
}

// ──────────────────────────────────────────
// SESION USUARIO
// ──────────────────────────────────────────

int SolicitarSesion(usuario a) {
    char linea[200];
    FILE *archivo = fopen(ARCHIVO_ADMINS, "r");
    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[100], apellido[100], correo[100], usr[100], pass[100];
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               nombre, apellido, correo, usr, pass);
        if (strcmp(usr, a.usr) == 0 && strcmp(pass, a.pass) == 0) {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}

// ──────────────────────────────────────────
// UI
// ──────────────────────────────────────────



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
    for (int i = 0; correo[i] != '\0'; i++) {
        if (correo[i] == '@') {
            for (int j = i; correo[j] != '\0'; j++) {
                if (correo[j] == '.') return 1;
            }
        }
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