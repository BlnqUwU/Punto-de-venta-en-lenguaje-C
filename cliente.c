#include "cliente.h"
#include "conexionipc.h"
#include "listas.h"
#include "utilidades.h"
#include <ncurses.h>
#include <string.h>

void Carrito(usuario u){
    int opcion = 0;
    int tecla,tecla2;
    

    char *menu[] = {
        "Pagar Carrito",
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);
    
    listaarticulo carrito;
    crearlistaarticulo(&carrito);

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char fecha[20];
    strftime(fecha, sizeof(fecha), "%d/%m/%Y %H:%M", tm_info);

    cargarCarrito(u.usr);
    carrito=ObtenerCarrito();

    articulo elegido;
    articulo auxtotal;
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
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        carrito=ObtenerCarrito();
        int n=carrito->NE;
        int salir=0;
        float totalpagar=0;
        
        ImprimirCentrado(5, "Punto de venta");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, (COLS/8)-strlen("producto")/2, "PRODUCTO");
        mvprintw(8, (COLS/2)-strlen("cantidad en carrito")/2, "CANTIDAD EN CARRITO");
        mvprintw(8, (int)(COLS/1.2)-strlen("precio")/2, "PRECIO");
        
        refresh();

        char pre[50];
        char cant[50];
        char total[50];
        int pos = 0;
        int totalamostrar=(LINES-11)-(LINES/4);
        if (opcion < n && opcion >= totalamostrar) {
            pos = opcion - totalamostrar + 1;
        } else if (opcion < n) {
            pos = 0;
        }

        if(!emptyarticulo(carrito)){ //verifica si el carrito esta vacio
            //imprime el carrito
        for (int i = pos; i < n && (i - pos) < totalamostrar; i++) {
            articulo ac = getarticulo(i, carrito);
            int fila = (LINES / 2) - (totalamostrar / 2) + (i - pos);

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
        //calcula el total a pagar del carrito
        for(int i=0; i<carrito->NE; i++){
            auxtotal=getarticulo(i,carrito);
            totalpagar+=auxtotal.precio*auxtotal.cantidad;
        }
        
        sprintf(total, "%.3f", totalpagar);
        }
        //imprime el total a pagar del carrito
        int col=(COLS/8) -strlen("Total a pagar: $")- strlen(total) / 2;
        if(col<0)
            col=0;
        mvprintw((LINES/2)+(totalamostrar/2)+3,col, "TOTAL A PAGAR: $%.3f", totalpagar);
        refresh();
        //imprime las opciones disponibles en menu
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
                if(opcion==n+1){ //salir seleccionado
                    guardarCarrito(u.usr);
                    liberarlistaarticulo(&carrito);
                    return;
                }else if(opcion==n){ //pagar carrito seleccionado
                    curs_set(0);
                    clear();

                    //mandar totalpagar a servidor para reporte de venta
                    venta v;;
                    v.total = totalpagar;
                    v.u = u;
                    strncpy(v.fecha, fecha, sizeof(v.fecha) - 1);
                    enviarVenta(v);

                    if(!emptyarticulo(carrito)){ //verifica si el carrito esta vacio
                        guardarCarrito(u.usr);
                        limpiarCarrito(); //elimina los elementos del carrito
                        ImprimirCentrado(LINES/2, "Carrito pagado con exito.");
                        getch();
                        opcion=0;
                        break;
                    }else{
                        ImprimirCentrado(LINES/2, "Nada por pagar.");
                        getch();
                        break;
                    }
                    
                }else{ //cualquier elemento del carrito elegido
                    elegido= getarticulo(opcion,carrito);
                    curs_set(0);
                    clear();
                    int col=(COLS/2)-strlen("Cuantas unidades de  desea quitar del carrito?: ")-strlen(elegido.producto);
                    if(col<0)
                        col=0;
                    mvprintw(LINES/2, col,
                            "Cuantas unidades de %s desea quitar del carrito?: ", elegido.producto);
                    mvprintw((LINES/2)+2, (COLS/2)-10, "%c: Agregar",24);
                    mvprintw((LINES/2)+2, (COLS/2)+10, "%c: Quitar",25);
                    mvprintw((LINES/2)+3, (COLS/2), "Salir: esc");
                    int cantidad=1;
                    //elegir cantidad a quitar del carrito
                    while(!salir){
                        move((LINES/2),col+strlen("Cuantas unidades de  desea quitar del carrito?: ")+strlen(elegido.producto));
                        clrtoeol();
                        mvprintw((LINES/2), (col+strlen("Cuantas unidades de  desea quitar del carrito?: ")+strlen(elegido.producto)) , "%d", cantidad);
                        tecla2=getch();
                        switch(tecla2) {
                            case KEY_UP:
                                cantidad++;
                                if(cantidad > elegido.cantidad) cantidad = 1;
                            break;

                            case KEY_DOWN:
                                cantidad--;
                                if(cantidad == 0) cantidad = elegido.cantidad;
                            break;
                            
                            case 10:
                                elegido.cantidad=elegido.cantidad-cantidad;
                                if(elegido.cantidad==0){ //si se quita toda la cantidad del carrito entonces lo quita del carrito y lo añade al catalogo
                                    enviararticulo(elegido, 3,1);
                                    articulo devolver = elegido;
                                    devolver.cantidad = obtenerCantidadCatalogo(elegido.producto) + cantidad;
                                    enviararticulo(devolver, 2,0);

                                    
                                    /*borrararticulo(opcion, carritoGlobal);
                                    addarticulo(opcion,elegido,cat);
                                    devolverExistencias(elegido.producto, cantidad); // ← falta*/
                                }else { 
                                    enviararticulo(elegido, 2, 1);
                                    articulo devolver2 = elegido;
                                    devolver2.cantidad = obtenerCantidadCatalogo(elegido.producto) + cantidad;
                                    enviararticulo(devolver2, 2,0);
                                }
                            
                                salir=1;
                            break;
                            case 27:// se preciono la tecla esc

                                endwin();
                            return;
                        }
                    }
                }
            break;
            
            case 27://se preciono la tecla esc
            liberarlistaarticulo(&carrito);
            endwin();
            
            return;
        }
    }
    return;
}

void Catalogo(usuario u){
    int opcion = 0;
    int tecla,tecla2;

    listaarticulo cat;
    crearlistaarticulo(&cat);

    char *menu[] = {
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);

    cargarCarrito(u.usr);
     //insertar catalogo de memoria compartida a lista
    //cargarCatalogo(cat);
    cat=ObtenerCatalogo();

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
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        //cat=ObtenerCatalogo();
        int n=cat->NE;
        int salir=0;
        int col=0;

        ImprimirCentrado(5, "Punto de venta");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, (COLS/8)-strlen("producto")/2, "PRODUCTO");
        mvprintw(8, (COLS/2)-strlen("cantidad disponible")/2, "CANTIDAD DISPONIBLE");
        mvprintw(8, (int)(COLS/1.2)-strlen("precio")/2, "PRECIO");
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
        if(!emptyarticulo(cat))//verifica si el catalogo esta vacio
        //imprime el catalogo
        for (int i = pos; i < n && (i - pos) < totalamostrar; i++) {
            articulo ac = getarticulo(i, cat);

            int fila = (LINES / 2) - (totalamostrar / 2) + (i - pos);

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
                    guardarCarrito(u.usr);
                    liberarlistaarticulo(&cat);
                    return;
                }else{//cualquier elemento del catalogo seleccionado
                    elegido= getarticulo(opcion,cat);
                    curs_set(0);
                    clear();
                    col=(COLS/2)-strlen("Cuantas unidades de  desea agregar al carrito?: ")-strlen(elegido.producto);
                    if(col<0)
                        col=0;
                    mvprintw(LINES/2, col,
                            "Cuantas unidades de %s desea agregar al carrito?: ", elegido.producto);
                    mvprintw((LINES/2)+2, (COLS/2)-10, "%c: Agregar",24);
                    mvprintw((LINES/2)+2, (COLS/2)+10, "%c: Quitar",25);
                    mvprintw((LINES/2)+3, (COLS/2), "Salir: esc");
                    int cantidad=1;
                    //seleccionar cuantos elementos agregar al carrito de compra desde el catalogo
                    while(!salir){
                        move((LINES/2),col+strlen("Cuantas unidades de  desea agregar al carrito?: ")+strlen(elegido.producto));
                        clrtoeol();
                        mvprintw((LINES/2), (col+strlen("Cuantas unidades de  desea agregar al carrito?: "))+strlen(elegido.producto), "%d", cantidad);
                        tecla2=getch();
                        switch(tecla2) {
                            case KEY_UP:
                                cantidad++;
                                if(cantidad > elegido.cantidad) cantidad = 1;
                            break;

                            case KEY_DOWN:
                                cantidad--;
                                if(cantidad == 0) cantidad = elegido.cantidad;
                            break;
                            
                            case 10:
                                elegido.cantidad=elegido.cantidad-cantidad;
                                enviararticulo(elegido, 2, 0);
                                elegido.cantidad=cantidad;
                                enviararticulo(elegido, 0, 1);

                                salir=1;
                                liberarlistaarticulo(&cat);
                                cat=ObtenerCatalogo();

                            break;
                            case 27:
                                salir=1;
                            liberarlistaarticulo(&cat);
                                endwin();
                            break;
                        }
                    }
                }
            break;
            
            case 27:
                guardarCarrito(u.usr);
                liberarlistaarticulo(&cat);
                endwin();
                return;
        }
    }
    liberarlistaarticulo(&cat);
    return;
}

int Perfil(usuario u){
    int opcion = 0;
    int tecla;
    int ver[5]={0,0,0,0,0};
    int ne = sizeof(ver)/sizeof(ver[0]);
    char *menu[] = {
        "Nombre:",
        "Apellido:",
        "Usuario:",
        "Correo:",
        "Password:",
        "Aceptar cambios",
        "Regresar"
    };

    int n = sizeof(menu)/sizeof(menu[0]);

    usuario login = obtenerUsuario(u);
    char usr_original[100];
    strncpy(usr_original, login.usr, sizeof(usr_original) - 1);
    char aux[100];

    char *DatosUsuario[]={
        login.nombre,
        login.apellido,
        login.usr,
        login.correo,
        login.pass
    };

    int l = sizeof(DatosUsuario)/sizeof(DatosUsuario[0]);

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
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        
        mvprintw((LINES/2)-3, ((COLS-strlen("Editar perfil"))/2), "Editar perfil");
        ImprimirCentrado((LINES/2)-2, "Selecciona una opcion");
        //imprime las opciones disponibles
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE); 
            if(i>4)
                ImprimirCentrado((LINES/2)+ i, menu[i]);
            else
                mvprintw((LINES/2)+ i, (COLS/2) -25, "%s", menu[i]);
            attroff(A_REVERSE);
        }

        refresh();
        //imprime los datos del usaurio menos la contraseña
        for(int i = 0; i < l-1; i++) {
                mvprintw((LINES/2)+ i, ((COLS/2)+strlen(menu[i])) -25, "%s", DatosUsuario[i]);
        }
        refresh();
        
        
        for(int i=0;i<5;i++){
            move((LINES/2)+11+i, 0);
            clrtoeol();
        }
        const char* mensajes[] = {
        "Nombre invalido, intente de nuevo.",
        "Apellido invalido, intente de nuevo.",
        "Nombre de usuario invalido, intente de nuevo.",
        "Correo invalido, intente de nuevo.",
        "Password invalida, intente de nuevo."
        };

        int pos = 0;
        //imprime mensajes de error si es que usuario ingreso datos invalidos en la iteracion anterior
        for (int i = 0; i < ne; i++) {
            if (ver[i] == 1) {
                move((LINES / 2) + 11 + pos, 0);
                clrtoeol();
                ImprimirCentrado((LINES / 2) + 11 + pos, mensajes[i]);
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
                    case 0://nombre elegido, guarda el nombre que se ingreso si es que es valido
                        ver[opcion]=0;
                        move((LINES/2)+opcion,8+(COLS/2)-26);
                        clrtoeol(); 
                        echo();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " "))
                            strcpy(login.nombre, aux);
                        else{
                            ver[opcion]=1;
                        }

                        noecho();
                        break;
                    case 1://apellido elegido, guarda el apellido que se ingreso si es que es valido
                        move((LINES/2)+opcion,10+(COLS/2)-26);
                        clrtoeol(); 
                        echo();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ")){
                            ver[opcion]=0;
                            strcpy(login.apellido, aux);
                        }
                        else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 2://nombre de usuario elegido, lo guarda si es valido
                        move((LINES/2)+opcion,9+(COLS/2)-26);
                        clrtoeol(); 
                        echo();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ") && BuscarUsuario(aux)==0){
                            ver[opcion]=0;
                            strcpy(login.usr, aux);
                        }
                        else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 3://correo elegido, lo guarda si es valido
                        move((LINES/2)+opcion,8+(COLS/2)-26);
                        clrtoeol(); 
                        echo();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ") && (VerificarCorreo(aux)==1) &&(BuscarCorreo(aux)==0)){
                            ver[opcion]=0;
                            strcpy(login.correo, aux);
                        }else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 4:// contraseña elegida, la guarda si es valida
                        ver[opcion]=0;
                        move((LINES/2)+opcion,10+(COLS/2)-26);
                        clrtoeol(); 
                        noecho();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ") && ComprobarPassword(aux)==5){
                            hash(aux, aux);
                            strcpy(login.pass, aux);
                        }else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 5://guardar datos elegido
                        curs_set(0);
                        clear();
                        if(enviarusuario(login, 2, usr_original)){ //mandar modificaciones al servidor (funcion en funciones_cliente)
                            ImprimirCentrado(LINES/2, "Cambios Guardados con exito, vuelve a iniciar sesion.");
                            getch();
                            clear();
                            return 1;
                        }else{
                            ImprimirCentrado(LINES/2, "Error al guardar los cambios.");
                            getch();
                            clear();
                            return 0;
                        }
                        break;
                    case 6:
                        return 0;
                }
                break;
            case 27:
            return 0;
        }
    }
    return 0;
}

void MenuPrincipal(usuario u){
    int opcion = 0;
    int tecla;
    char *menu[] = {
        "Ver catalogo",
        "Ver carrito de compra",
        "Ver perfil",
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
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();
        
        mvprintw((LINES/2)-3, ((COLS-strlen("!Bienvenido ")-strlen(u.usr))/2), "!Bienvenido %s!", u.usr);
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
                        Catalogo(u);
                        break;
                    case 1:
                        Carrito(u);
                        break;
                    case 2:
                        if (Perfil(u)==1) //si hubo cambios en el perfil y fueron guardados correctamente entonces regresa al inicio de sesion
                            return;
                        else//si no entonces regresa a esta ventana
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


void registrar() {

    //initscr();
    set_escdelay(0);
    curs_set(0);
    keypad(stdscr, TRUE);
    usuario user = {"","","","",""};
    clear();
    echo();
    char pass[50], hash_pass[100];
    int opcion = 0;
    int tecla;
    int col= 0;
    int cor=0;
    int us=0;
    int pas=0;
    char aux0[50],aux1[50];
    char *menu[] = {
        "Nombre:",
        "Apellido:",
        "Correo:",
        "Usuario:",
        "Password:",
        "Registrar",
        "Regresar "
    };
    
    int n = sizeof(menu)/sizeof(menu[0]);
    
    char *DatosUsuario[]={
        user.nombre,
        user.apellido,
        user.correo,
        user.usr,
        user.pass
    };

    int l = sizeof(DatosUsuario)/sizeof(DatosUsuario[0]);
    
    while(1) {
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

        ImprimirCentrado((LINES/2) -3, "Registrar un usuario");
        //imprime las opciones del menu
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            if(i>=5) 
                ImprimirCentrado((LINES/2)+i, menu[i]);
            else
                mvprintw((LINES/2) + i, (COLS/2) -25, "%s", menu[i]);

            attroff(A_REVERSE);
        }

        refresh();
        //imprime los datos ingresados por el usuario, exceptuando la contraseña
        for(int i = 0; i < l-1; i++) {
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

        col= (COLS - strlen(aux1)-strlen("La password no cumple con los requerimientos")) / 2;
        if(pas==1){
        if(cor!=0||us==1){
            if(cor&&us){
                move((LINES/2)+(LINES/4)+5, 0);
                clrtoeol();
                mvprintw((LINES/2)+(LINES/4)+5, col, "La password no cumple con los requerimientos");
            }else if ((cor==1&&cor==2)!=us){
                move((LINES/2)+(LINES/4)+4, 0);
                clrtoeol();
                mvprintw((LINES/2)+(LINES/4)+4, col, "La password no cumple con los requerimientos");
            }
        }else{
                move((LINES/2)+(LINES/4)+3, 0);
                clrtoeol();
                mvprintw((LINES/2)+(LINES/4)+3, col, "La password no cumple con los requerimientos");
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
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
                        strcpy(user.correo, "");
                        clrtoeol();
                        cor=1;
                    }else if(BuscarCorreo(user.correo) == 1){
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+7+(LINES/4),8+49);
                        clrtoeol();
                        cor=2;
                        strcpy(aux0, user.correo);
                        strcpy(user.correo, "");
                    }else {
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
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
                    if (BuscarUsuario(user.usr) == 1){
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+8+(LINES/4),9+49);
                        clrtoeol();
                        us=1;
                        strcpy(aux1, user.usr);
                        strcpy(user.usr, "");
                    }else {
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
                        clrtoeol();
                    }
                    break;
                    case 4: //contraseña elegida, las restricciones son impresas
                    pas=0;
                    noecho();
                    move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                    move((LINES/2)+2+(LINES/4), 0);
                        clrtoeol();
                    move((LINES/2)+3+(LINES/4),0);
                        clrtoeol();
                    ImprimirCentrado((LINES/2)+1+(LINES/4), "La password debe tener minimo 12 caracteres de largo." );
                    ImprimirCentrado((LINES/2)+2+(LINES/4), "La password debe ser una combinacion de mayusculas, minusculas, numeros y simbolos.");
                    ImprimirCentrado((LINES/2)+3+(LINES/4), "(por ejemplo: ! # $ & , - . )");
                    move((LINES/2)+4,10+(COLS/2)-26);
                    clrtoeol(); 
                    getstr(pass);
                    noecho();

                    if (ComprobarPassword(pass) < 5) {
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+3+(LINES/4), 0);
                        clrtoeol();
                        noecho();
                        pas=1;
                        ImprimirCentrado((LINES/2)+11, "La password no cumple con los requerimientos.");
                        noecho();
                    }else {
                        move((LINES/2)+1+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+2+(LINES/4), 0);
                        clrtoeol();
                        move((LINES/2)+3+(LINES/4), 0);
                        clrtoeol();
                        echo();
                        hash(pass, hash_pass);
                        strncpy(user.pass, hash_pass, sizeof(user.pass));
                    }
                        
                    break;
                    case 5:
                    curs_set(0);
                        clear();
                        //verifica que no halla datos sin llenar
                        if((strcmp(user.pass, "")==0)||(strcmp(user.correo, "")==0)||(strcmp(user.usr, "")==0)||(strcmp(user.usr, " ")==0)||(strcmp(user.nombre, "")==0)){
                            mvprintw(3, 10, "Datos incompletos.");
                            getch();
                            clear();
                            
                            break;
                        }else{
                            if (enviarusuario(user, 0, NULL) == 1) { //mandar registro a servidor (funcion en funciones_cliente)
                                mvprintw(3, 10, "Registro Exitoso");
                                getch();
                                clear();
                                return;
                            }else {
                                mvprintw(3, 10, "Registro fallido.");
                                getch();
                                clear();
                                return;
                            }
                        }
                    case 6:
                    return;
                    
            
                }
                curs_set(0);
                break;
            case 27:
            return;
            
        }
    }
}

void iniciarSesion() {

    //initscr();
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
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        curs_set(0);
        clear();

        ImprimirCentrado((LINES/2) -3, "Iniciar Sesion");

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
            ImprimirCentrado((LINES/2)+(LINES/4), "Usuario y/o password incorrectos.");
        
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

                    if(solicitarSesion(user) == 1) { //pedir la sesion al servidor (funcion en funciones_cliente), si usuario existe en base de datos entra al menu principal
                        clear();
                        MenuPrincipal(obtenerUsuario(user));
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
        "Registro",
        "Salir"
    };

    int n = 3;

    //inicia la pantalla, quita el retraso al boton esc, activa no mostrar el input y activa la deteccion del teclado
    initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); 

    //si la terminal admite color, inicia los colores negro y cyan para la interfaz




    while(1) {

        curs_set(0);
        clear();
        if (!conectarServidor()) {
            ServidorSinConexion();
        }
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_BLACK, COLOR_CYAN);
        }
        bkgd(COLOR_PAIR(1));
        ImprimirCentrado((LINES/2) -3, "Punto de venta");
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
                    registrar();
                    break;
                }
                if(opcion == 2){
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
    if (!conectarServidor()) {
        ServidorSinConexion();
    }
    menu();
    desconectarServidor();
    return 0;
}