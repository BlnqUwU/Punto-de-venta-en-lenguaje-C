



// ──────────────────────────────────────────
// 1. CARGAR CATALOGO DESDE SHM A LISTA
//    Sustituye: //insertar catalogo de memoria compartida a lista
// ──────────────────────────────────────────

void cargarCatalogo(listaarticulo cat) {

    if (!Ishm) {
        printf("DEBUG: shm es NULL\n");
        return;
    }
    printf("DEBUG: totalProductos = %d\n", Ishm->totalProductos);

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm->totalProductos; i++) {
        Producto *p = &Ishm->productos[i];

        if (!p->activo || p->existencias <= 0)
            continue;

        infoarticulo item;
        strncpy(item.producto, p->nombre, sizeof(item.producto) - 1);
        item.producto[sizeof(item.producto) - 1] = '\0';
        item.cantidad = p->existencias;
        item.precio   = p->precio;

        addarticulo(cat->NE, item, cat);
    }

    upSem(semID, SEM_INV);
}

// ──────────────────────────────────────────
// 2. ACTUALIZAR EXISTENCIAS EN SHM
//    Sustituye: //actualizar catalogo servidor
// ──────────────────────────────────────────

void actualizarExistencias(const char *nombreProducto, int cantidadVendida) {
    if (!Ishm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm->totalProductos; i++) {
        Producto *p = &Ishm->productos[i];

        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->existencias -= cantidadVendida;
            if (p->existencias < 0)
                p->existencias = 0;
            break;
        }
    }

    upSem(semID, SEM_INV);

    // NOTIFICAR AL SERVIDOR QUE HAY CAMBIOS
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

// ──────────────────────────────────────────
// 3. REGISTRAR VENTA EN SHM
//    Sustituye: //mandar totalpagar a servidor para reporte de venta
// ──────────────────────────────────────────

void registrarVenta(float total, const char *usr) {
    if (!Ishm) return;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char fecha[20];
    strftime(fecha, sizeof(fecha), "%d/%m/%Y %H:%M", tm_info);


    downSem(semID, SEM_INV);

    if (Ishm->totalVentas < MAX_VENTAS) {
        Venta v;
        v.total = total;
        strncpy(v.usr,   usr,   sizeof(v.usr)   - 1);
        strncpy(v.fecha, fecha, sizeof(v.fecha) - 1);
        v.usr[sizeof(v.usr)     - 1] = '\0';
        v.fecha[sizeof(v.fecha) - 1] = '\0';

        Ishm->ventas[Ishm->totalVentas] = v;
        Ishm->totalVentas++;
    }

    upSem(semID, SEM_INV);

    FILE *f = fopen("ventas.txt", "a");
    if (f) {
        fprintf(f, "%s,%.2f,%s\n", usr, total, fecha);
        fclose(f);
    }

    // NOTIFICAR AL SERVIDOR PARA QUE GUARDE
    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}

void devolverExistencias(const char *nombreProducto, int cantidad) {
    if (!Ishm) return;

    downSem(semID, SEM_INV);

    for (int i = 0; i < Ishm->totalProductos; i++) {
        Producto *p = &Ishm->productos[i];
        if (p->activo && strcmp(p->nombre, nombreProducto) == 0) {
            p->existencias += cantidad;
            break;
        }
    }

    upSem(semID, SEM_INV);

    upSem(semID, SEM_REQ);
    downSem(semID, SEM_ACK);
}
int cargarUsuario(usuario u) {
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    if (archivo == NULL) return 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[50], apellido[50], correo[50], usr[50], pass[100];
        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               nombre, apellido, correo, usr, pass);
        if(strcmp(usr, u.usr) == 0 && strcmp(pass, u.pass) == 0) {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}