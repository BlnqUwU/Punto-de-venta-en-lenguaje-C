#include "admin.h"
#include "conexionipc.h"
#include "listas.h"
#include "memoria_compartida.h"
#include <ncurses.h>
#include <string.h>

void AgregarProducto(){
        set_escdelay(0);
    curs_set(0);
    keypad(stdscr, TRUE);
    articulo producto = {"",0,0};
    articulo aux = {"",0,0};
    clear();
    echo();
    int opcion = 0;
    int tecla;
    int col= 0;
    int ver[4]={0,0,0,0};
    int ne = sizeof(ver)/sizeof(ver[0]);
    char aux0[50],aux1[50],aux2[100];
    char *menu[] = {
        "Producto:",
        "Cantidad:",
        "Precio:",
        "Agregar producto",
        "Regresar "
    };
    char pre[50];
    char cant[50];
    sprintf(pre, "%.2f", producto.precio);
    sprintf(cant, "%d", producto.cantidad);
    int n = sizeof(menu)/sizeof(menu[0]);

    char *DatosProducto[]={
        producto.producto,
        cant,
        pre

    };

    int l = sizeof(DatosProducto)/sizeof(DatosProducto[0]);

    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();

        ImprimirCentrado((LINES/2) -3, "Agregar un producto");
        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            if(i>=3)
                ImprimirCentrado((LINES/2)+i, menu[i]);
            else
                mvprintw((LINES/2) + i, (COLS/2) -25, "%s", menu[i]);

            attroff(A_REVERSE);
        }

        refresh();
        //imprime los datos ingresados por el usuario, exceptuando la contraseña
        for(int i = 0; i < l; i++) {
            mvprintw((LINES/2)+ i, ((COLS/2)+strlen(menu[i])) -25, "%s", DatosProducto[i]);
        }

        refresh();
        const char* mensajes[] = {
        "Producto invalido, intente de nuevo",
        "Cantidad invalida, intente de nuevo.",
        "Precio invalido, intente de nuevo.",
        "Datos incompletos"
        };

        int pos = 0;
        //imprime mensajes de error si es que usuario ingreso datos invalidos en la iteracion anterior
        for (int i = 0; i < ne; i++) {
            if (ver[i] == 1) {
                move((LINES / 2) +(LINES/4) + pos+2, 0);
                clrtoeol();
                ImprimirCentrado((LINES / 2) + (LINES/4) + pos+2, mensajes[i]);
                pos++; 
            }
        }
        refresh();

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n - 1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n) opcion = 0;
                break;

            case 10: // ENTER
            curs_set(1);
                switch (opcion) {
                    case 0://nombre seleccionado, sin restriccion
                    move((LINES/2),strlen(menu[opcion])+(COLS/2)-25);
                    clrtoeol();
                    echo();
                    getstr (aux2);
                    strcpy(aux.producto, aux2);
                        if(BuscarProducto(aux2)||strcmp(aux2, "")==0||strcmp(aux2, " ")==0) {
                            ver[0]=1;
                        }
                    else{
                        strcpy(producto.producto, aux2);
                        ver[0]=0;
                    }
                    noecho();
                    break;
                    case 1://apellido seleccionado, sin restriccion
                    move((LINES/2)+1,strlen(menu[opcion])+(COLS/2)-25);
                    clrtoeol();
                    echo();
                    getstr (aux0);
                    int cf=atoi(aux0);
                    if(cf<=0)
                        ver[1]=1;
                    else{
                        sprintf(cant, "%d",cf);
                        producto.cantidad=cf;
                        ver[1]=0;
                    }
                    noecho();
                    break;
                    case 2://correo seleccionado, debe contener @ y . despues del @, tambien no debe de existir en la base de datos
                    move((LINES/2)+2,strlen(menu[opcion])+(COLS/2)-25);
                    clrtoeol();
                    echo();
                    getstr (aux1);
                    float pf= (float)atof(aux1);
                    if(pf<=0)
                        ver[2]=1;
                    else{
                        sprintf(pre, "%0.2f",pf);
                        producto.precio=pf;
                        ver[2]=0;
                    }
                    noecho();
                    break;
                    case 3://usuario seleccionado, no debe existir en la base de datos
                    if(strcmp(producto.producto, "")==0||strcmp(producto.producto, " ")==0||producto.precio<=0||producto.cantidad<=0){

                        ver[3]=1;
                        break;
                    }else{
                        enviararticulo(producto, 0, 0);
                        //agregarProductoAdmin(producto);
                        ver[3]=0;
                    }
                    return;
                    case 4: //contraseña elegida, las restricciones son impresas
                    return;
                }
                curs_set(0);
                break;
            case 27:
            return;

        }
    }
}

void EditarUsuario(usuario user){
    set_escdelay(0);
    curs_set(0);
    keypad(stdscr, TRUE);
    clear();
    echo();
    int opcion = 0;
    int tecla;
    int col= 0;
    int cor=0;
    int us=0;
    char aux0[50],aux1[50];
    char *usr[]={user.usr};
    char *menu[] = {
        "Nombre:",
        "Apellido:",
        "Correo:",
        "Usuario:",
        "Aplicar cambios",
        "Eliminar usuario",
        "Regresar "
    };

    int n = sizeof(menu)/sizeof(menu[0]);

    char *DatosUsuario[]={
        user.nombre,
        user.apellido,
        user.correo,
        user.usr,
    };

    int l = sizeof(DatosUsuario)/sizeof(DatosUsuario[0]);

    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();

        ImprimirCentrado((LINES/2) -3, "Editar un usuario");
        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            if(i>=4)
                ImprimirCentrado((LINES/2)+i, menu[i]);
            else
                mvprintw((LINES/2) + i, (COLS/2) -25, "%s", menu[i]);

            attroff(A_REVERSE);
        }

        refresh();
        //imprime los datos ingresados por el usuario, exceptuando la contraseña
        for(int i = 0; i < l; i++) {
            mvprintw((LINES/2)+ i, ((COLS/2)+strlen(menu[i])) -25, "%s", DatosUsuario[i]);
        }

        move((LINES/2)+11, 0);
        clrtoeol();
        //imprime mensajes de error si el usuario ingreso datos invalidos en la iteracion anterior
        if(cor==1){
            
            ImprimirCentrado((LINES/2)+(LINES/4)+3, "Correo Invalido");
        }
        else if (cor==2){
            mvprintw((LINES/2)+(LINES/4)+3, (((COLS-strlen("El correo  ya esta en uso")-strlen(aux0))/2)), "El Correo %s ya esta en uso", aux0);
        }
            
        col= (COLS - strlen(aux1)-strlen("El usuario  ya existe")) / 2;
        if(col<0)
            col=0;
        if(us==1){  
            if(cor!=0){
                move((LINES/2)+(LINES/4)+4, 0);
                clrtoeol();
                mvprintw((LINES/2)+(LINES/4)+4, col, "El usuario %s ya existe", aux1);
            } else{
                move((LINES/2)+(LINES/4)+3,col);
                clrtoeol(); 
                mvprintw((LINES/2)+(LINES/4)+3, col, "El usuario %s ya existe", aux1);
            }
            
        }


        refresh();

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n - 1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n) opcion = 0;
                break;

            case 10: // ENTER
            curs_set(1);
                switch (opcion) {
                    case 0://nombre seleccionado, sin restriccion
                    move((LINES/2),8+(COLS/2)-26);
                    clrtoeol();
                    echo();
                    getstr (user.nombre);
                    noecho();
                    break;
                    case 1://apellido seleccionado, sin restriccion
                    move((LINES/2)+1,10+(COLS/2)-26);
                    clrtoeol();
                    echo();
                    getstr (user.apellido);
                    noecho();
                    break;
                    case 2://correo seleccionado, debe contener @ y . despues del @, tambien no debe de existir en la base de datos
                    cor=0;
                    move((LINES/2)+2,8+(COLS/2)-26);
                    clrtoeol();
                    echo();
                    getstr (user.correo);
                    noecho();
                    if (VerificarCorreo(user.correo) == 0){
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        strcpy(user.correo, "");
                        clrtoeol();
                        cor=1;
                    }else if (BuscarCorreo(user.correo) == 1) {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                        move((LINES/2)+7,8+49);
                        clrtoeol();
                        cor=2;
                        strcpy(aux0, user.correo);
                        strcpy(user.correo, "");
                    }else {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                    }
                    break;
                    case 3://usuario seleccionado, no debe existir en la base de datos
                    us=0;
                    echo();
                    move((LINES/2)+3,9+(COLS/2)-26);
                    clrtoeol();
                    getstr (user.usr);
                    noecho();
                    if (BuscarUsuario(user.usr) == 1) {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                        move((LINES/2)+8,9+49);
                        clrtoeol();
                        us=1;
                        strcpy(aux1, user.usr);
                        strcpy(user.usr, "");
                    }else {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                    }
                    break;

                    case 4:
                    curs_set(0);
                        clear();
                        //verifica que no halla datos sin llenar
                        if((strcmp(user.pass, "")==0)||(strcmp(user.correo, "")==0)||(strcmp(user.usr, "")==0)||(strcmp(user.usr, " ")==0)||(strcmp(user.nombre, "")==0)){
                            mvprintw(3, 10, "Datos incompletos.");
                            getch();
                            clear();

                            break;
                        }else{
                            if (enviarusuario(user,2, NULL) == 1) { //mandar registro a servidor (funcion en funciones_cliente)
                                mvprintw(3, 10, "Cambios registrados.");
                                getch();
                                clear();
                                return;
                            }else {
                                mvprintw(3, 10, "Cambio fallido.");
                                getch();
                                clear();
                                return;
                            }
                        }
                    case 5:
                        curs_set(0);
                        clear();
                        if (enviarusuario(user, 3, NULL) == 1) { //mandar registro a servidor (funcion en funciones_cliente)
                            mvprintw(3, 10, "Cambios registrados.");
                            getch();
                            clear();
                            return;
                        }else {
                            mvprintw(3, 10, "Cambio fallido.");
                            getch();
                            clear();
                            return;
                        }
                    return;
                    case 6:
                    return;


                }
                curs_set(0);
                break;
            case 27:
            return;

        }
    }
    return;
}

void ventas(listaventa ventas){
    int opcion = 0;
    int tecla,tecla2;

    char *menu[] = {
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);

    articulo elegido;
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);


    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        int n=ventas->NE;
        int salir=0;


        ImprimirCentrado(5, "Punto de venta (administrador)");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, (COLS/4)-strlen("FECHA")/2, "FECHA");
        mvprintw(8, (int)(COLS/1.34) -strlen("VENDIDO")/2, "VENDIDO");
        refresh();

        char pre[50];
        char cant[50];
        int pos = 0;
        char aux[50];
        int totalamostrar=(LINES-11)-(LINES/4);
        if (opcion < n && opcion >= totalamostrar) {
            pos = opcion - totalamostrar + 1;
        } else if (opcion < n) {
            pos = 0;
        }
        if(!emptyventa(ventas)){
        //imprime el catalogo
        for (int i = pos; i < n && (i - pos) < totalamostrar; i++) {

            infoventa ac = getventa(i, ventas);
            int fila = (LINES / 2) - (totalamostrar / 2) + (i - pos)+1;

            move(fila, 0);
            clrtoeol();

            if (i == opcion)
                attron(A_REVERSE);

            sprintf(pre, "%.2f", ac.v.total);
            mvprintw(fila, (COLS/4) - strlen(ac.v.fecha) / 2, "%s", ac.v.fecha);
            mvprintw(fila,(int)(COLS/1.34) - strlen(pre) / 2, "$%.2f", ac.v.total);

            attroff(A_REVERSE);
        }
    }
        refresh();
        //imprime el boton de salir

        for (int i = 0; i < m; i++) {
            if(i+n==opcion)
                attron(A_REVERSE);

            ImprimirCentrado((LINES/2)+(totalamostrar/2)+4+i, menu[i]);

            attroff(A_REVERSE);

        }

        refresh();


        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n+1) opcion = 0;
                break;

            case 10: // ENTER
                if(opcion==n){//salir seleccionado
                    return;
                }
            break;

            case 27:
            endwin();

            return;
        }
    }
    return;
}

void GenerarReportes(){
    int opcion = 0;
    int tecla;

    char *menu[] = {
        "Generar reporte diario",
        "Generar reporte semanal",
        "Generar reporte mensual",
        "Salir"
    };
    listaventa ventasemanal;
    crearlistaventa(&ventasemanal);


    listaventa ventadiaria;
    crearlistaventa(&ventadiaria);

    listaventa ventamensual;
    crearlistaventa(&ventamensual);



    int n = sizeof(menu)/sizeof(menu[0]);

    //initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();

        ImprimirCentrado(5, "Punto de venta (administrador)");
        ImprimirCentrado(6, "Selecciona una opcion");

        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            ImprimirCentrado((LINES/2)+ i, menu[i]);
            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n - 1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n) opcion = 0;
                break;

            case 10: // ENTER

                switch (opcion) {
                    case 0://reporte diario
                        ventadiaria=obtenerVentas(0);
                        ventas(ventadiaria);
                        break;
                    case 1://reporte semanal
                        ventasemanal=obtenerVentas(1);
                        ventas(ventasemanal);
                        break;
                    case 2://reporte mensual
                        ventamensual=obtenerVentas(2);
                        ventas(ventamensual);
                        break;
                    case 3:
                        liberarlistaventa(&ventadiaria);
                        liberarlistaventa(&ventasemanal);
                        liberarlistaventa(&ventamensual);
                        return;
                }
                break;
            case 27:
            liberarlistaventa(&ventadiaria);
            liberarlistaventa(&ventasemanal);
            liberarlistaventa(&ventamensual);
            return;
        }
    }
    liberarlistaventa(&ventadiaria);
    liberarlistaventa(&ventasemanal);
    liberarlistaventa(&ventamensual);
    return;
}
void AdministrarCatalogo(){
    int opcion = 0;
    int tecla,tecla2;

    listaarticulo cat;
    crearlistaarticulo(&cat);
    char *menu[] = {
        "Agregar Producto",
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);

    cat = ObtenerCatalogo();
    articulo elegido;
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);


    while(1) {
        //cargarCatalogoAdmin(cat);
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        int n=cat->NE;
        int salir=0;


        ImprimirCentrado(5, "Punto de venta (administrador)");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, (COLS/8)-strlen("producto")/2, "PRODUCTO");
        mvprintw(8, (COLS/2)-strlen("cantidad disponible")/2, "CANTIDAD DISPONIBLE");
        mvprintw(8, (int)(COLS/1.2)-strlen("precio")/2, "PRECIO");
        refresh();

        char pre[50];
        char cant[50];
        int pos = 0;
        char aux[50];
        int totalamostrar=(LINES-11)-(LINES/4);
        if (opcion < n && opcion >= totalamostrar) {
            pos = opcion - totalamostrar + 1;
        } else if (opcion < n) {
            pos = 0;
        }
        if(!emptyarticulo(cat)){//verifica si el catalogo esta vacio
        //imprime el catalogo
        for (int i = pos; i < n && (i - pos) < totalamostrar; i++) {

            articulo ac = getarticulo(i, cat);
            int fila = (LINES / 2) - (totalamostrar / 2) + (i - pos)+1;

            move(fila, 0);
            clrtoeol();

            if (i == opcion)
                attron(A_REVERSE);

            sprintf(pre, "%.2f", ac.precio);
            sprintf(cant, "%d", ac.cantidad);
            mvprintw(fila, (COLS/8) - strlen(ac.producto) / 2, "%s", ac.producto);
            mvprintw(fila, (COLS/2) - strlen(cant) / 2, "%d", ac.cantidad);
            mvprintw(fila, (int)(COLS/1.2) - strlen(pre) / 2, "$%.2f", ac.precio);

            attroff(A_REVERSE);
        }
    }
        refresh();
        //imprime el boton de salir

        for (int i = 0; i < m; i++) {
            if(i+n==opcion)
                attron(A_REVERSE);

            ImprimirCentrado((LINES/2)+(totalamostrar/2)+4+i, menu[i]);

            attroff(A_REVERSE);

        }



        refresh();


        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n+1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n+2) opcion = 0;
                break;

            case 10: // ENTER
                if(opcion==n+1){//salir seleccionado
                    liberarlistaarticulo(&cat);
                    return;
                }else if(opcion==n){
                    AgregarProducto();
                    liberarlistaarticulo(&cat);
                    cat = ObtenerCatalogo();
                    opcion=0;
                    break;
                }else{//cualquier elemento del catalogo seleccionado
                    elegido= getarticulo(opcion,cat);
                    curs_set(0);
                    clear();
                    int col=(COLS/2)-strlen("Cuantas unidades de  desea quitar o agregar al catalogo?: ")-strlen(elegido.producto);
                    if(col<0)
                        col=0;
                    mvprintw(LINES/2, col,
                            "Cuantas unidades de %s desea quitar o agregar al catalogo?: ", elegido.producto);
                    ImprimirCentrado((LINES/2)+2, "Flecha arriba: Agregar         Flecha abajo:Quitar");
                    ImprimirCentrado((LINES/2)+3, "Salir: esc");
                    int cantidad=0;
                    while (!salir) {
                        move((LINES/2),(COLS/2)+strlen("Cuantas unidades de  desea quitar o agregar al carrito?: ")+strlen(elegido.producto));
                        clrtoeol();
                        mvprintw((LINES/2), (col+strlen("Cuantas unidades de  desea quitar o agregar al carrito?: ")+strlen(elegido.producto)) , "%d", cantidad);
                        tecla2=getch();
                        switch (tecla2) {
                            case KEY_UP:
                                cantidad++;
                                //if(cantidad > elegido.cantidad) cantidad = 1;
                            break;

                            case KEY_DOWN:
                                cantidad--;
                                //if(cantidad == 0) cantidad = elegido.cantidad;
                            break;
                            case 10:
                                if((elegido.cantidad+cantidad)<=0){
                                    enviararticulo(elegido, 3, 0);
                                    curs_set(0);
                                    clear();
                                    ImprimirCentrado(LINES/2, "Producto eliminado con exito.");
                                    getch();
                                    return;
                                }else{
                                    elegido.cantidad += cantidad;
                                    enviararticulo(elegido, 2, 0);
                                    curs_set(0);
                                    clear();
                                    ImprimirCentrado(LINES/2, "Existencias actualizadas con exito.");
                                    getch();
                                        return;
                                }
                                salir=1;
                            break;
                            case 27:
                                salir=1;
                                endwin();
                            break;

                        }
                    }
                }
            break;

            case 27:
            endwin();

            return;
        }
    }
    return;
}

void AdministrarUsuarios(){
    int opcion = 0;
    int tecla,tecla2;

    lista usuarios;
    crearlista(&usuarios);
    char *menu[] = {
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);

    usuarios=ObtenerUsuarios();

    //insertar catalogo de memoria compartida a lista

    info elegido;
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);


    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        int n=usuarios->NE;
        int salir=0;
        usuarios=ObtenerUsuarios();

        ImprimirCentrado(5, "Punto de venta");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, (COLS/8)-strlen("nombre")/2, "Nombre");
        mvprintw(8, (int)(COLS/2.854)-strlen("apellido")/2, "Apellido");
        mvprintw(8, (int)(COLS/1.734)-strlen("correo")/2, "Correo");
        mvprintw(8, (int)(COLS/1.141)-strlen("usuario")/2, "Usuario");
        refresh();

        char pre[50];
        char cant[50];
        int pos = 0;
        int totalamostrar=(LINES-11)-(LINES/4);
        if (opcion < n && opcion >= totalamostrar) {
            pos = opcion - totalamostrar + 1;
        } else if (opcion < n) {
            pos = 0;
        }
        if(!empty(usuarios))//verifica si hay usuarios
        //imprime el catalogo
        for (int i = pos; i < n && (i - pos) < totalamostrar; i++) {
            info ac = get(i, usuarios);
            int fila = (LINES / 2) - (totalamostrar / 2) + (i - pos)+1;

            move(fila, 0);
            clrtoeol();

            if (i == opcion)
                attron(A_REVERSE);

            mvprintw(fila, (COLS/8) - strlen(ac.u.nombre) / 2, "%s", ac.u.nombre);
            mvprintw(fila, (int)(COLS/2.854) - strlen(ac.u.apellido) / 2, "%s", ac.u.apellido);
            mvprintw(fila, (int)(COLS/1.734) - strlen(ac.u.correo) / 2, "%s", ac.u.correo);
            mvprintw(fila, (int)(COLS/1.141) - strlen(ac.u.usr) / 2, "%s", ac.u.usr);

            attroff(A_REVERSE);
        }

        refresh();
        //imprime el boton de salir
            if(opcion ==n)
                attron(A_REVERSE);

            ImprimirCentrado((LINES/2)+(totalamostrar/2)+3, menu[0]);

            attroff(A_REVERSE);


        refresh();


        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n+1) opcion = 0;
                break;

            case 10: // ENTER
                if(opcion==n){//salir seleccionado
                    liberarlista(&usuarios);
                    return;
                }else{//cualquier usuario seleccionado
                    elegido=get(opcion, usuarios);
                    EditarUsuario(elegido.u);
                }
            break;

            case 27:
                liberarlista(&usuarios);
                endwin();
            return;
        }
    }
    liberarlista(&usuarios);
    return;
}

void MenuPrincipal(char *usuario){
    int opcion = 0;
    int tecla;
    char *menu[] = {
        "Administrar usuarios",
        "Administrar catalogo",
        "Generar reportes de venta",
        "Cerrar sesion"
    };

    int n = sizeof(menu)/sizeof(menu[0]);

    //initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();

        mvprintw((LINES/2)-3, ((COLS-strlen("!Bienvenido ")-strlen(usuario))/2), "!Bienvenido %s!", usuario);
        ImprimirCentrado((LINES/2)-2, "Selecciona una opcion");

        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            ImprimirCentrado((LINES/2)+ i, menu[i]);
            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n - 1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n) opcion = 0;
                break;

            case 10: // ENTER

                switch (opcion) {
                    case 0:
                        AdministrarUsuarios();
                        break;
                    case 1:
                        AdministrarCatalogo();
                        break;
                    case 2:
                        GenerarReportes();
                            break;
                    case 3:
                        return;
                }
                break;
            case 27:
            return;
        }
    }
}

void iniciarSesion() {

    set_escdelay(0);
    noecho();
    clear();
    curs_set(0);
    keypad(stdscr, TRUE);

    int errorCount = 0;
    usuario user = {"","","","",""};

    char pass[50], hash_pass[50];

    int opcion = 0;
    int tecla;

    char *menu[] = {
        "Usuario:",
        "Password:",
        "Entrar",
        "Regresar"
    };

    int n = sizeof(menu)/sizeof(menu[0]);

    char *DatosUsuario[]={
        user.usr,
        user.pass
    };

    int l = sizeof(DatosUsuario)/sizeof(DatosUsuario[0]);

    while(errorCount != 3) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        ImprimirCentrado((LINES/2) -3, "Iniciar Sesion (Administrador)");

        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            if(i>=2)
                ImprimirCentrado((LINES/2)+i, menu[i]);
            else
                mvprintw((LINES/2) + i, (COLS/2) -25, "%s", menu[i]);
            attroff(A_REVERSE);
        }
        refresh();

        //imprime datos del usuario menos la contraseña
        for(int i = 0; i < l; i++) {
            mvprintw((LINES/2)+i, ((COLS/2)+strlen(menu[i])) -25, "%s", DatosUsuario[i]);
        }

        if(errorCount>0) //imprime error si es que los datos no son correctos
            ImprimirCentrado((LINES/2) +(LINES/4), "Usuario y/o password incorrectos.");

        refresh();

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n - 1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n) opcion = 0;
                break;

            case 10: // ENTER
            curs_set(1);
                switch (opcion) {
                    case 0://leer usuario
                    move((LINES/2),9+(COLS/2) -26);
                    clrtoeol();
                    echo();
                    getstr (user.usr);
                    noecho();
                    break;
                    case 1://leer contraseña
                    move((LINES/2) +1,10+(COLS/2) -26);
                    clrtoeol();
                    noecho();
                    getstr (pass);
                    break;
                    case 2://Entrar seleccionado
                    curs_set(0);
                    hash(pass, hash_pass);
                    strncpy(user.pass, hash_pass, sizeof(user.pass));

                    if(solicitarSesionAdmin(user) == 1) { //pedir la sesion al servidor (funcion en funciones_cliente), si usuario existe en base de datos entra al menu principal
                        clear();
                        MenuPrincipal(user.usr);
                        return;
                    }else{
                        strcpy(user.usr, "");
                        move((LINES/2),9+(COLS/2) -26);
                        clrtoeol();
                        strcpy(user.pass, "");
                        move((LINES/2) +1,10+(COLS/2) -26);
                        clrtoeol();
                        opcion=0;
                        errorCount++;
                        clear();
                    }
                    if(errorCount == 3 && errorCount >0) { //limite de intentos alcanzado, sale de la ventana de inicio de sesion
                        clear();
                        ImprimirCentrado((LINES/2), "3 intentos fallidos.");
                        getch();
                        endwin();
                        return;
                    }
                    break;
                    case 3:
                    return;

                }
               curs_set(0);
                break;
            case 27:
            curs_set(0);
            noecho();
            return;
        }
    }
}

void menu() {
    int opcion = 0;
    int tecla;

    char *menu[] = {
        "Login",
        "Salir"
    };

    int n = sizeof(menu)/sizeof(menu[0]);

    //inicia la pantalla, quita el retraso al boton esc, activa no mostrar el input y activa la deteccion del teclado
    initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    //si la terminal admite color, inicia los colores negro y azul para la interfaz
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_BLUE);
    }

    bkgd(COLOR_PAIR(1));

    while(1) {
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_BLUE);
        }

        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();


        ImprimirCentrado((LINES/2) -3, "Punto de Venta (Administrador)");
        ImprimirCentrado((LINES/2) -2, "Selecciona una opcion");

        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);

            ImprimirCentrado((LINES/2)+i, menu[i]);
            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch(tecla) {
            case KEY_UP:
                opcion--;
                if(opcion < 0) opcion = n - 1;
                break;

            case KEY_DOWN:
                opcion++;
                if(opcion >= n) opcion = 0;
                break;

            case 10: // ENTER

                if(opcion == 0) {
                    iniciarSesion();
                    break;
                }
                if(opcion == 1){
                    endwin();
                    return;
                }
                break;
            case 27:
            endwin();

            return;
        }
    }
}

void ServidorSinConexion(){
    initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
        clear();
    int tecla;
    keypad(stdscr, TRUE); 
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_MAGENTA);
    }
    
    while (1) {
        bkgd(COLOR_PAIR(1));
        if(conectarServidor()){
            curs_set(0);
            clear();
            endwin();
            return;
        }
            curs_set(0);
        clear();
            ImprimirCentrado((LINES/2), "Sin conexion al servidor.");
            tecla=getch();
            switch (tecla) {
                case 27:
                    endwin();
                    exit(1);
                default:
                    break;                
            
        }
        
    }


}

int main() {
    if (conectarServidor() == 0) {
        ServidorSinConexion();
    }
    crearAdminSiNoExiste();
    menu();
    desconectarServidor();
    return 0;
}