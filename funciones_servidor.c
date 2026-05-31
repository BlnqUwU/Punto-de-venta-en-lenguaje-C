#include <complex.h>

#include "listas.h"
#include "memoria_compartida.h"
#include "utilidades_back.h"

int BuscarAtributo(char *atributo, char *BD){
    return 0;
}

int EnviarSesion(usuario u, char *BD){

    char linea[300];
    FILE *archivo = fopen(BD, "r");
    if (!archivo) return -1;

    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[100], apellido[100], correo[100], usr[100], pass[100];
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               nombre, apellido, correo, usr, pass);

        if (strcmp(usr, u.usr) == 0 && strcmp(pass, u.pass) == 0) {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return -1;
}

listaarticulo EnviarCatalogo(){
    listaarticulo catalogo;

    return catalogo;
}
lista EnviarUsuarios(){
    lista usuarios;
    crearlista(&usuarios);
    char linea[300];
    FILE *archivo = fopen(ARCHIVO_USR, "r");
    if (!archivo) return usuarios;

    int cont = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        usuario u;
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               u.nombre, u.apellido, u.correo, u.usr, u.pass);

        info inf;
        inf.u = u;
        add(cont, inf, usuarios);
        cont++;
    }
    fclose(archivo);
    return usuarios;
}


void CRUDusuario(usuarioShm *Ushm, int CRUD){

    // CRUD: 0=crear 1=leer/sesion 2=actualizar 3=borrar

    if (CRUD == 0) {
        //CREAR
        FILE *archivo = fopen(ARCHIVO_USR, "a");
        if (!archivo) { Ushm -> realizado = -1; return; }

        usuario u = Ushm -> u;
        fprintf(archivo, "%s,%s,%s,%s,%s,\n",
                u.nombre, u.apellido, u.correo, u.usr, u.pass);
        fclose(archivo);
        if (Ushm -> totalUsuarios < MAX_USUARIOS) {
            Ushm -> usuarios[Ushm -> totalUsuarios] = u;
            Ushm -> totalUsuarios++;
        }
        Ushm -> realizado = 1;
    } else if (CRUD == 1) {
        //LEER
        for (int i = 0; i < Ushm -> totalUsuarios; i++) {
            if (strcmp(Ushm -> usuarios[i].usr, Ushm -> u.usr) == 0) {
                if (strcmp(Ushm -> usuarios[i].pass, "") != 0) {
                    if (strcmp(Ushm -> usuarios[i].pass, Ushm -> u.pass) != 0) {
                        Ushm -> realizado = -1;
                        return;
                    }
                }
                Ushm -> u = Ushm -> usuarios[i];
                Ushm -> realizado = 1;
                return;
            }
        }
        Ushm -> realizado = -1;
    } else if (CRUD == 2) {
        // ACTUALIZAR

        char *buscar = (strlen(Ushm->usr_original) > 0) ? Ushm->usr_original : Ushm->u.usr;

        for (int i = 0; i < Ushm -> totalUsuarios; i++) {
            if (strcmp(Ushm -> usuarios[i].usr, buscar) == 0) {
                Ushm -> usuarios[i] = Ushm -> u;

                FILE *original = fopen(ARCHIVO_USR, "r");
                FILE *temp = fopen("temp_usr.txt", "w");

                if (original && temp) {
                    char linea[300];
                    while (fgets(linea, sizeof(linea), original)) {
                        char usr[100];
                        sscanf(linea, "%*[^,],%*[^,],%*[^,],%[^,]", usr);
                        if (strcmp(usr, buscar) == 0) {
                            fprintf(temp, "%s,%s,%s,%s,%s,\n",
                                        Ushm->u.nombre, Ushm->u.apellido,
                                        Ushm->u.correo, Ushm->u.usr, Ushm->u.pass);
                        } else {
                            fputs(linea, temp);
                        }
                    }
                    fclose(original);
                    fclose(temp);
                    remove(ARCHIVO_USR);
                    rename("temp_usr.txt", ARCHIVO_USR);
                }
                Ushm -> usr_original[0] = '\0';
                Ushm -> realizado = 1;
                return;
            }
        }
        Ushm -> realizado = -1;

    } else if (CRUD == 3) {
        // BORRAR
        for (int i = 0; i < Ushm -> totalUsuarios; i++) {
            if (strcmp(Ushm -> usuarios[i].usr, Ushm -> u.usr) == 0) {
                Ushm -> usuarios[i] = Ushm -> usuarios[Ushm -> totalUsuarios - 1];
                Ushm -> totalUsuarios--;

                FILE *original = fopen(ARCHIVO_USR, "r");
                FILE *temp = fopen("temp_usr.txt", "w");

                if (original && temp) {
                    char linea[300];
                    while (fgets(linea, sizeof(linea), original)) {
                        char usr[100];
                        sscanf(linea, "%*[^,],%*[^,],%*[^,],%[^,]", usr);
                        if (strcmp(usr, Ushm -> u.usr) != 0) fputs(linea, temp);
                    }
                    fclose(original);
                    fclose(temp);
                    remove(ARCHIVO_USR);
                    rename("temp_usr.txt", ARCHIVO_USR);
                }
                Ushm -> realizado = 1;
                return;
            }
        }
        Ushm -> realizado = -1;
    }
}

void CRUDcatalogo(InventarioShm *Ishm, int CRUD, int BD){

    //CATALOGO
    if (BD == 0) {
        if (CRUD == 0) {
            if (Ishm -> totalCatalogo >= MAX_PRODUCTOS) {
                Ishm -> realizado = -1;
                return;
            }

            Ishm -> catalogo[Ishm -> totalCatalogo] = Ishm -> p;
            Ishm -> totalCatalogo++;

            FILE *archivo = fopen(ARCHIVO_CAT, "a");
            if (!archivo) { Ishm -> realizado = -1; return; }

            fprintf(archivo, "%s,%d,%.2f,",
                Ishm->p.producto, Ishm->p.cantidad, Ishm->p.precio);
            fclose(archivo);
            Ishm -> realizado = 1;
        } else if (CRUD == 1) {
            //LEER

            FILE *archivo = fopen(ARCHIVO_CAT, "r");
            if (!archivo) { Ishm -> realizado = -1; return; }

            Ishm -> totalCatalogo = 0;
            char linea[300];

            while (fgets(linea, sizeof(linea), archivo) && Ishm -> totalCatalogo < MAX_PRODUCTOS) {
                articulo a;
                char cantidad[20], precio[20];
                sscanf(linea, "%[^,],%[^,],%[^,\n]", a.producto, cantidad, precio);
                a.cantidad = atoi(cantidad);
                a.precio = atof(precio);
                Ishm -> catalogo[Ishm -> totalCatalogo] = a;
                Ishm -> totalCatalogo++;
            }
            fclose(archivo);
            Ishm -> realizado = 1;
        }else if (CRUD == 2) {
            //ACTUALIZAR
            for (int i = 0; i < Ishm -> totalCatalogo; i++) {
                if (strcmp(Ishm -> catalogo[i].producto, Ishm -> p.producto) == 0) {
                    Ishm -> catalogo[i] = Ishm -> p;

                    FILE *original = fopen(ARCHIVO_CAT, "r");
                    FILE *temp = fopen("temp_catalo.txt", "w");
                    if (original && temp) {
                        char linea[300];
                        while (fgets(linea, sizeof(linea), original)) {
                            char nombre[100];
                            sscanf(linea, "%[^,]", nombre);
                            if (strcmp(nombre, Ishm -> p.producto) == 0) {
                                fprintf(temp, "%s,%d,%.2f,\n",
                                    Ishm->p.producto, Ishm->p.cantidad, Ishm->p.precio);
                            } else {
                                fputs(linea, temp);
                            }
                        }
                        fclose(temp);
                        fclose(original);
                        remove(ARCHIVO_CAT);
                        rename("temp_catalo.txt", ARCHIVO_CAT);
                        Ishm -> realizado = 1;
                        return;
                    }
                }
                Ishm -> realizado = -1;
            }
        } else if (CRUD == 3) {
            //BORRAR
            for (int i = 0; i < Ishm -> totalCatalogo; i++) {
                if (strcmp(Ishm -> catalogo[i].producto, Ishm -> p.producto) == 0) {
                    Ishm -> catalogo[i] = Ishm -> catalogo[Ishm -> totalCatalogo - 1];
                    Ishm -> totalCatalogo--;

                    FILE *original = fopen(ARCHIVO_CAT, "r");
                    FILE *temp = fopen("temp_catalo.txt", "w");

                    if (original && temp) {
                        char linea[300];
                        while (fgets(linea, sizeof(linea), original)) {
                            char nombre[100];
                            sscanf(linea, "%[^,]", nombre);
                            if (strcmp(nombre, Ishm -> p.producto) != 0) {
                                fputs(linea, temp);
                            }
                        }
                        fclose(temp);
                        fclose(original);
                        remove(ARCHIVO_CAT);
                        rename("temp_catalo.txt", ARCHIVO_CAT);
                        Ishm -> realizado = 1;
                        return;
                    }
                }
                Ishm -> realizado = -1;
            }
        }
    } else if (Ishm -> BD == 1) {
        return;
    }
}

void CRUDventas(ventaShm *Vshm, int CRUD){

    // 0=crear, 1=leer

    if (CRUD == 0) {
        if (Vshm -> totalVentas >= MAX_VENTAS) {
            Vshm -> realizado = -1;
            return;
        }

        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char fecha[20];
        strftime(fecha, sizeof(fecha), "%d/%m/%Y %H:%M", tm_info);
        strncpy(Vshm->v.fecha, fecha, sizeof(Vshm->v.fecha) - 1);

        Vshm -> ventas[Vshm -> totalVentas] = Vshm -> v;
        Vshm -> totalVentas++;

        FILE *archivo = fopen(ARCHIVO_VD, "a");
        if (archivo) {
            fprintf(archivo, "%s,%.2f,%s,",
                    Vshm->v.u.usr, Vshm->v.total, fecha);

            fclose(archivo);
        }
        Vshm -> realizado = 1;
    } else if (CRUD == 1) {
        // tipo: 0=diario 1=semanal 2=mensual

        int dias = 1;
        if (Vshm -> tipo == 1) dias = 7;
        else if (Vshm -> tipo == 2) dias = 30;

        char *archivo_ventas = ARCHIVO_VD;

        FILE *archivo = fopen(archivo_ventas, "r");
        if (!archivo) {Vshm -> totalVentas = 0; Vshm -> realizado = 1; return;}

        Vshm -> totalVentas = 0;
        char linea[300];
        time_t ahora = time(NULL);

        while (fgets(linea, sizeof(linea), archivo) && Vshm->totalVentas < MAX_VENTAS) {
            char usr[100], fecha[20];
            float total;
            sscanf(linea, "%[^,],%f,%[^,]", usr, &total, fecha);

            struct tm tm_venta = {0};
            sscanf(fecha, "%d/%d/%d %d:%d",
                   &tm_venta.tm_mday, &tm_venta.tm_mon,
                   &tm_venta.tm_year, &tm_venta.tm_hour, &tm_venta.tm_min);
            tm_venta.tm_mon  -= 1;
            tm_venta.tm_year -= 1900;
            tm_venta.tm_isdst = -1;

            time_t t_venta = mktime(&tm_venta);
            double diff = difftime(ahora, t_venta);

            if (diff >= 0 && diff <= dias * 86400.0) {
                venta v;
                v.total = total;
                strncpy(v.u.usr, usr, sizeof(v.u.usr) - 1);
                strncpy(v.fecha, fecha, sizeof(v.fecha) - 1);
                Vshm->ventas[Vshm->totalVentas] = v;
                Vshm->totalVentas++;
            }
        }
        fclose(archivo);
        Vshm->realizado = 1;
    }
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