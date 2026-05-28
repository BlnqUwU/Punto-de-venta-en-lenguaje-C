#include "admin.h"
#include "listas.h"

#define ARCHIVO_ADMINS "admins.txt"

// ──────────────────────────────────────────
// HASH DJB2
// ──────────────────────────────────────────

void hash(char *input, char *output) {
    unsigned long h = 5381;
    int c;
    while ((c = *input++))
        h = ((h << 5) + h) + c;
    sprintf(output, "%lu", h);
}

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
    FILE *archivo = fopen(arch, "a");
    if (archivo != NULL) {
        fprintf(archivo, "%s,", strcmp(u.nombre, "") == 0 ? " " : u.nombre);
        fprintf(archivo, "%s,", strcmp(u.apellido, "") == 0 ? " " : u.apellido);
        fprintf(archivo, "%s,", u.correo);
        fprintf(archivo, "%s,", u.usr);
        fprintf(archivo, "%s,", u.pass);
        fprintf(archivo, "\n");
        fclose(archivo);
        return 1;
    }
    return 0;
}

int ModificarAtributo(usuario u, char *usr) {
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    usuario temp = {"","","","",""};
    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               temp.nombre, temp.apellido, temp.correo, temp.usr, temp.pass);
        if (strcmp(temp.usr, usr) == 0)
            RegistrarUsuario(u, "temp.txt");
        else
            RegistrarUsuario(temp, "temp.txt");
    }
    fclose(archivo);
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
    return 1;
}

int ModificarAtributoCatalogo(usuario u, char *usr) {
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    usuario temp = {"","","","",""};
    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               temp.nombre, temp.apellido, temp.correo, temp.usr, temp.pass);
        if (strcmp(temp.usr, usr) == 0)
            RegistrarUsuario(u, "temp.txt");
        else
            RegistrarUsuario(temp, "temp.txt");
    }
    fclose(archivo);
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
    return 1;
}

int BorrarUsuario(usuario a, char *usr){
    //solicitar a servidor borrar este usuario
    //si lo borra retorna 1 si no retorna 0
    return 0;
}

// ──────────────────────────────────────────
// OBTENER LISTAS
// ──────────────────────────────────────────

lista ObtenerUsuarios() {
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    lista usuarios;
    crearlista(&usuarios);
    usuario user = {"","","","",""};
    info usr;
    int cont = 0;

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               user.nombre, user.apellido, user.correo, user.usr, user.pass);
        usr.a = user;
        add(cont, usr, usuarios);
        cont++;
    }
    fclose(archivo);
    return usuarios;
}

listaarticulo ObtenerProductos() {
    char linea[200];
    FILE *archivo = fopen("catalogo.txt", "r");
    listaarticulo productos;
    crearlistaarticulo(&productos);
    producto prod_temp = {"", 0, 0};
    producto fin;
    int cont = 0;
    char cantidad[50], precio[50];

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "%[^,],%[^,],%[^,\n]",
               prod_temp.producto, cantidad, precio);
        prod_temp.cantidad = atoi(cantidad);
        prod_temp.precio   = atof(precio);
        fin.p = prod_temp;
        addproducto(cont, fin, productos);
        cont++;
    }
    fclose(archivo);
    return productos;
}

int RegistrarProducto(prod u, char *arch) {
    FILE *archivo = fopen(arch, "a");
    if (archivo != NULL) {
        fprintf(archivo, "%s,%d,%f,\n", u.p.producto, u.p.cantidad, u.p.precio);
        fclose(archivo);
        return 1;
    }
    return 0;
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
    char linea[100];
    FILE *archivo = fopen("usuarios.txt", "r");
    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) {
            char *token = strtok(linea, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            while (token != NULL) {
                if (strcmp(token, correo) == 0) {
                    fclose(archivo);
                    return 1;
                }
                token = strtok(NULL, ",");
            }
        }
        fclose(archivo);
    }
    return 0;
}

int BuscarUsuario(char *usr) {
    char linea[100];
    FILE *archivo = fopen("usuarios.txt", "r");
    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) {
            char *token = strtok(linea, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            while (token != NULL) {
                if (strcmp(token, usr) == 0) {
                    fclose(archivo);
                    return 1;
                }
                token = strtok(NULL, ",");
            }
        }
        fclose(archivo);
    }
    return 0;
}

int BuscarProducto(char *producto) {
    char linea[100];
    FILE *archivo = fopen("catalogo.txt", "r");
    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) {
            char *token = strtok(linea, ",");
            while (token != NULL) {
                if (strcmp(token, producto) == 0) {
                    fclose(archivo);
                    return 1;
                }
                token = strtok(NULL, ",");
            }
        }
        fclose(archivo);
    }
    return 0;
}

// CARGAR VENTAS POR RANGO DE DIAS
// dias=1 diario, dias=7 semanal, dias=30 mensual
// ──────────────────────────────────────────

void cargarVentasPorRango(listaventa lv, int dias) {
    FILE *f = fopen("ventas.txt", "r");
    if (!f) return;

    time_t ahora = time(NULL);
    char linea[200];
    int cont = 0;

    while (fgets(linea, sizeof(linea), f)) {
        char usr[100], fecha[20];
        float total;

        // formato: usr,total,DD/MM/YYYY HH:MM
        sscanf(linea, "%[^,],%f,%[^\n]", usr, &total, fecha);

        // parsear fecha
        struct tm tm_venta = {0};
        sscanf(fecha, "%d/%d/%d %d:%d",
               &tm_venta.tm_mday, &tm_venta.tm_mon,
               &tm_venta.tm_year, &tm_venta.tm_hour, &tm_venta.tm_min);
        tm_venta.tm_mon  -= 1;    // tm_mon va de 0-11
        tm_venta.tm_year -= 1900; // tm_year desde 1900
        tm_venta.tm_isdst = -1;

        time_t t_venta = mktime(&tm_venta);
        double diff = difftime(ahora, t_venta);

        // filtrar por rango en segundos
        if (diff >= 0 && diff <= dias * 86400.0) {
            infoventa iv;
            iv.v.total = total;
            strncpy(iv.v.fecha, fecha, sizeof(iv.v.fecha) - 1);
            iv.v.fecha[sizeof(iv.v.fecha) - 1] = '\0';
            addventa(cont, iv, lv);
            cont++;
        }
    }
    fclose(f);
}