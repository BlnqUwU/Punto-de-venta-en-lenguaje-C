#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cliente.h"


void Carrito(char *usuario){
    int opcion = 0;
    int tecla,tecla2;
    

    lista carrito;
    crearlista(&carrito);
    char *menu[] = {
        "Pagar Carrito",
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);

    
    //insertar lista de compra de memoria compartida a lista
    
    lista cat;
    crearlista(&cat);
    info elegido;
    info auxtotal;
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); 


    while(1) {
        curs_set(0);
        clear();
        int n=carrito->NE;
        int salir=0;
        float totalpagar=0;
        
        ImprimirCentrado(5, "Punto de venta");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, 25-strlen("producto")/2, "PRODUCTO");
        mvprintw(8, 100-strlen("cantidad en carrito")/2, "CANTIDAD EN CARRITO");
        mvprintw(8, 175-strlen("precio")/2, "PRECIO");
        
        refresh();

        char pre[50];
        char cant[50];
        char total[50];
        int pos = 0;
        if (opcion < n && opcion >= 30) {
            pos = opcion - 30 + 1;
        } else if (opcion < n) {
            pos = 0;
        }

        if(!empty(carrito)){
        for (int i = pos; i < n && (i - pos) < 30; i++) {
            info ac = get(i, carrito);
            int fila = (LINES / 2) - (30 / 2) + (i - pos);

            move(fila, 0);
            clrtoeol();

            if (i == opcion)
                attron(A_REVERSE);
            
            sprintf(pre, "%.2f", ac.precio);
            sprintf(cant, "%d", ac.cantidad);
            mvprintw(fila, 25 - strlen(ac.producto) / 2, "%s", ac.producto);
            mvprintw(fila, 100 - strlen(cant) / 2, "%d", ac.cantidad);
            mvprintw(fila, 174 - strlen(pre) / 2, "$%.2f", ac.precio);
            
            attroff(A_REVERSE);
        }
        
        for(int i=0; i<carrito->NE; i++){
            auxtotal=get(i,carrito);
            totalpagar+=auxtotal.precio*auxtotal.cantidad;
        }

        sprintf(total, "%.3f", totalpagar);
        }
        mvprintw(41, 25 -strlen("Total a pagar: $")- strlen(total) / 2, "TOTAL A PAGAR: $%.3f", totalpagar);
        refresh();
        
        for (int i = 0; i < m; i++) {
            if(i+n==opcion)
                attron(A_REVERSE); 
            
            ImprimirCentrado(42+i, menu[i]);
            
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
                if(opcion==n+1){
                    liberarlista(&carrito);
                    liberarlista(&cat);
                    return;
                }else if(opcion==n){
                    curs_set(0);
                    clear();
                    //mandar totalpagar a servidor para reporte de venta
                    if(!empty(carrito)){
                        Vaciarlista(carrito);
                        ImprimirCentrado(LINES/2, "Carrito pagado con exito.");
                        getch();
                        opcion=0;
                        break;
                    }else{
                        ImprimirCentrado(LINES/2, "Nada por pagar.");
                        getch();
                        break;
                    }
                    
                }else{
                    elegido= get(opcion,carrito);
                    curs_set(0);
                    clear();
                    mvprintw(LINES/2, (COLS/2)-strlen("Cuantas unidades de  desea quitar del carrito?: "), 
                            "Cuantas unidades de %s desea quitar del carrito?: ", elegido.producto);
                    mvprintw((LINES/2)+2, (COLS/2)-10, "%c: Agregar",24);
                    mvprintw((LINES/2)+2, (COLS/2)+10, "%c: Quitar",25);
                    int cantidad=0;
                    while(!salir){
                        move((LINES/2),(COLS/2)+strlen("Cuantas unidades de  desea quitar del carrito?: "));
                        clrtoeol();
                        mvprintw((LINES/2), ((COLS/2)+strlen("Cuantas unidades de  desea quitar del carrito?: ")) -25, "%d", cantidad);
                        tecla2=getch();
                        switch(tecla2) {
                            case KEY_UP:
                                cantidad--;
                                if(cantidad < 0) cantidad = elegido.cantidad;
                            break;

                            case KEY_DOWN:
                                cantidad++;
                                if(cantidad > elegido.cantidad) cantidad = 0;
                            break;
                            
                            case 10:
                                elegido.cantidad=elegido.cantidad-cantidad;
                                if(elegido.cantidad==0){
                                    borrar(opcion, carrito);
                                    add(opcion,elegido,cat);
                                }else { 
                                    set(opcion, elegido, carrito);

                                }
                            
                                salir=1;
                            break;
                            case 27:
                                liberarlista(&carrito);
                                liberarlista(&cat);
                                endwin();
                            return;
                        }
                    }
                }
            break;
            
            case 27:
            liberarlista(&carrito);
            liberarlista(&cat);
            endwin();
            
            return;
        }
    }
    return;
}

void Catalogo(char *usuario){
    int opcion = 0;
    int tecla,tecla2;

    lista cat;
    crearlista(&cat);
    char *menu[] = {
        "Salir"
    };
    int m= sizeof(menu)/sizeof(menu[0]);

     //insertar catalogo de memoria compartida a lista
    
    info elegido;
    lista carrito;
    crearlista(&carrito);
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); 


    while(1) {
        curs_set(0);
        clear();
        int n=cat->NE;
        int salir=0;
        
        ImprimirCentrado(5, "Punto de venta");
        ImprimirCentrado(6, "Selecciona una opcion");

        mvprintw(8, 25-strlen("producto")/2, "PRODUCTO");
        mvprintw(8, 100-strlen("cantidad disponible")/2, "CANTIDAD DISPONIBLE");
        mvprintw(8, 175-strlen("precio")/2, "PRECIO");
        refresh();

        char pre[50];
        char cant[50];
        int pos = 0;
        if (opcion < n && opcion >= 30) {
            pos = opcion - 30 + 1;
        } else if (opcion < n) {
            pos = 0;
        }
        if(!empty(cat))
        for (int i = pos; i < n && (i - pos) < 30; i++) {
            info ac = get(i, cat);
            int fila = (LINES / 2) - (30 / 2) + (i - pos);

            move(fila, 0);
            clrtoeol();

            if (i == opcion)
                attron(A_REVERSE);
            
            sprintf(pre, "%.2f", ac.precio);
            sprintf(cant, "%d", ac.cantidad);
            mvprintw(fila, 25 - strlen(ac.producto) / 2, "%s", ac.producto);
            mvprintw(fila, 100 - strlen(cant) / 2, "%d", ac.cantidad);
            mvprintw(fila, 174 - strlen(pre) / 2, "$%.2f", ac.precio);
            
            attroff(A_REVERSE);
        }
        
        refresh();
            if(opcion ==n)
                attron(A_REVERSE); 
            
            ImprimirCentrado(41, menu[0]);
            
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
                if(opcion==n){
                    liberarlista(&carrito);
                    liberarlista(&cat);
                    return;
                }else{
                    elegido= get(opcion,cat);
                    curs_set(0);
                    clear();
                    mvprintw(LINES/2, (COLS/2)-strlen("Cuantas unidades de  desea agregar al carrito?: "), 
                            "Cuantas unidades de %s desea agregar al carrito?: ", elegido.producto);
                    mvprintw((LINES/2)+2, (COLS/2)-10, "%c: Agregar",24);
                    mvprintw((LINES/2)+2, (COLS/2)+10, "%c: Quitar",25);
                    int cantidad=0;
                    while(!salir){
                        move((LINES/2),(COLS/2)+strlen("Cuantas unidades de  desea agregar al carrito?: "));
                        clrtoeol();
                        mvprintw((LINES/2), ((COLS/2)+strlen("Cuantas unidades de  desea agregar al carrito?: ")) -25, "%d", cantidad);
                        tecla2=getch();
                        switch(tecla2) {
                            case KEY_UP:
                                cantidad--;
                                if(cantidad < 0) cantidad = elegido.cantidad;
                            break;

                            case KEY_DOWN:
                                cantidad++;
                                if(cantidad > elegido.cantidad) cantidad = 0;
                            break;
                            
                            case 10:
                                elegido.cantidad=elegido.cantidad-cantidad;
                                if(elegido.cantidad==0){
                                    borrar(opcion, cat);
                                }else { 
                                set(opcion, elegido, cat);
                                //actualizar catalogo servidor
                                add(carrito->NE,elegido,carrito);
                                //añadir a carrito de compra servidor
                                }
                            
                                salir=1;
                            break;
                            case 27:
                                endwin();
                            break;
                        }
                    }
                }
            break;
            
            case 27:
            liberarlista(&carrito);
            liberarlista(&cat);
            endwin();
            
            return;
        }
    }
    liberarlista(&carrito);
    liberarlista(&cat);
    return;
}

int Perfil(char *usr){
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

    usuario login = SolicitarPerfil(usr);
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
        curs_set(0);
        clear();
        
        mvprintw((LINES/2)-3, ((COLS-strlen("Editar perfil"))/2), "Editar perfil");
        ImprimirCentrado((LINES/2)-2, "Selecciona una opcion");
        
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

        for(int i = 0; i < l; i++) {
            if(i==4)
                break;
            else
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
                    case 0:
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
                    case 1:
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
                    case 2:
                        
                        move((LINES/2)+opcion,9+(COLS/2)-26);
                        clrtoeol(); 
                        echo();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ")){
                            ver[opcion]=0;
                            strcpy(login.usr, aux);
                        }
                        else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 3:
                        
                        move((LINES/2)+opcion,8+(COLS/2)-26);
                        clrtoeol(); 
                        echo();
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ") && VerificarCorreo(aux)==1){
                            ver[opcion]=0;
                            strcpy(login.correo, aux);
                        }else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 4:
                        ver[opcion]=0;
                        move((LINES/2)+opcion,10+(COLS/2)-26);
                        clrtoeol(); 
                        noecho();
                        //getstr (login.pass);
                        getstr (aux);
                        if(strcmp(aux,"") && strcmp(aux, " ") && ComprobarPassword(aux)==5){
                            hash(aux, aux);
                            strcpy(login.pass, aux);
                        }else{
                            ver[opcion]=1;
                        }
                        noecho();
                        break;
                    case 5:

                        curs_set(0);
                        clear();
                        if(ModificarAtributo(login, usr)){ //mandar modificaciones al servidor (funcion en funciones_cliente)
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

void MenuPrincipal(char *usuario){
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
        curs_set(0);
        clear();
        
        mvprintw((LINES/2)-3, ((COLS-strlen("!Bienvenido ")-strlen(usuario))/2), "!Bienvenido %s!", usuario);
        ImprimirCentrado((LINES/2)-2, "Selecciona una opcion");
        //mvprintw(3, 10, "Selecciona una opcion:");
        ImprimirCentrado(2, "");
        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE); 
            ImprimirCentrado((LINES/2)+ i, menu[i]);
            //mvprintw((LINES/2)+ i, 50, "%s", menu[i]);
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
                        Catalogo(usuario);
                        break;
                    case 1:
                        Carrito(usuario);
                        break;
                    case 2:
                        if (Perfil(usuario)==1)
                            return;
                        else
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
        curs_set(0);
        clear();

        ImprimirCentrado((LINES/2) -3, "Registrar un usuario");
        //mvprintw(3, 10, "Registrar un usuario");

        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);//arreglar para no imprimir la contrasena
            if(i>=5) 
                ImprimirCentrado((LINES/2)+i, menu[i]);
            else
                mvprintw((LINES/2) + i, (COLS/2) -25, "%s", menu[i]);

            attroff(A_REVERSE);
        }

        refresh();

        for(int i = 0; i < l; i++) {
            mvprintw((LINES/2)+ i, ((COLS/2)+strlen(menu[i])) -25, "%s", DatosUsuario[i]);
        }

        move((LINES/2)+11, 0);
        clrtoeol();
        if(cor==1){
            
            ImprimirCentrado((LINES/2)+11, "Correo Invalido");
        }
        else if (cor==2){
            mvprintw((LINES/2)+11, (((COLS-strlen("El correo  ya esta en uso")-strlen(aux0))/2)), "El Correo %s ya esta en uso", aux0);
        }
            
        col= (COLS - strlen(aux1)-strlen("El usuario  ya existe")) / 2;
        if(col<0)
            col=0;
        if(us==1){  
            if(cor!=0){
                move((LINES/2)+12, 0);
                clrtoeol();
                mvprintw((LINES/2)+12, col, "El usuario %s ya existe", aux1);  
            } else{
                move((LINES/2)+11,col);
                clrtoeol(); 
                mvprintw((LINES/2)+11, col, "El usuario %s ya existe", aux1);  
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
                    case 0:
                    move((LINES/2),8+(COLS/2)-26);
                    clrtoeol(); 
                    echo();
                    getstr (user.nombre);
                    noecho();
                    break;
                    case 1:
                    move((LINES/2)+1,10+(COLS/2)-26);
                    clrtoeol(); 
                    echo();
                    getstr (user.apellido);
                    noecho();
                    break;
                    case 2:
                    cor=0;
                    move((LINES/2)+2,8+(COLS/2)-26);
                    clrtoeol(); 
                    echo();
                    getstr (user.correo);
                    noecho();
                    if (VerificarCorreo(user.correo) == 0) {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        strcpy(user.correo, "");
                        clrtoeol();
                        cor=1;
                        //mvprintw(15, 10, "Correo Invalido");
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
                    case 3:
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
                    noecho();
                    ImprimirCentrado((LINES/2)+11, "La password debe tener minimo 12 caracteres de largo." );
                    ImprimirCentrado((LINES/2)+12, "La password debe ser una combinacion de mayusculas, minusculas, numeros y simbolos.");
                    ImprimirCentrado((LINES/2)+13, "(por ejemplo: ! # $ & , - . )");
                    move((LINES/2)+4,10+(COLS/2)-26);
                    clrtoeol(); 
                    getstr(pass);
                    noecho();

                    if (ComprobarPassword(pass) < 5) {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                        move((LINES/2)+13, 0);
                        clrtoeol();
                        noecho();
                        ImprimirCentrado((LINES/2)+11, "La password no cumple con los requerimientos.");
                        noecho();
                    }else {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                        move((LINES/2)+13, 0);
                        clrtoeol();
                        echo();
                        hash(pass, hash_pass);
                        strncpy(user.pass, hash_pass, sizeof(user.pass));
                    }
                        
                    break;
                    case 5:
                    curs_set(0);
                        clear();
                        if((strcmp(user.pass, "")==0)||(strcmp(user.correo, "")==0)||(strcmp(user.usr, "")==0)||(strcmp(user.nombre, "")==0)){
                            mvprintw(3, 10, "Datos incompletos.");
                            getch();
                            clear();
                            
                            break;
                        }else{
                            if (RegistrarUsuario(user,"usuarios.txt") == 1) { //mandar registro a servidor (funcion en funciones_cliente)
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
        curs_set(0);
        clear();
        ImprimirCentrado((LINES/2) -3, "Iniciar Sesion");
        //mvprintw(3, 10, "Iniciar Sesion");

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

        for(int i = 0; i < l; i++) {
            mvprintw((LINES/2)+i, ((COLS/2)+strlen(menu[i])) -25, "%s", DatosUsuario[i]);
        }

        if(errorCount>0)
            ImprimirCentrado((LINES/2) +11, "Usuario y/o password incorrectos.");
        
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
                    case 0:
                    move((LINES/2),9+(COLS/2) -26);
                    clrtoeol(); 
                    echo();
                    getstr (user.usr);
                    noecho();
                    break;
                    case 1:
                    move((LINES/2) +1,10+(COLS/2) -26);
                    clrtoeol(); 
                    noecho();
                    getstr (pass);
                    break;
                    case 2:
                    curs_set(0);
                    hash(pass, hash_pass);
                    strncpy(user.pass, hash_pass, sizeof(user.pass));

                    if(SolicitarSesion(user) == 1) { //pedir la sesion al servidor (funcion en funciones_cliente)
                        clear();
                        MenuPrincipal(user.usr);
                        return;
                    }else{
                        //mvprintw(10, 10, "Datos incorrectos.");
                        strcpy(user.usr, "");
                        move((LINES/2),9+(COLS/2) -26);
                        clrtoeol();
                        strcpy(user.pass, "");
                        move((LINES/2) +1,10+(COLS/2) -26);
                        clrtoeol(); 
                        errorCount++;
                        clear();
                    }
                    if(errorCount == 3 && errorCount >0) {
                        clear();
                        ImprimirCentrado((LINES/2), "3 intentos fallidos.");
                        //mvprintw(3, 10, "3 Intentos Fallidos");
                        getch();
                        endwin();
                        return;
                    }else{
                        ImprimirCentrado((LINES/2) +11, "Usuario y/o password incorrectos.");
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

    initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); 

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_CYAN);
    }

    bkgd(COLOR_PAIR(1));

    while(1) {
        curs_set(0);
        clear();

        //mvprintw(3, 10, "Selecciona una opcion:");

        ImprimirCentrado((LINES/2) -3, "Punto de venta");
        ImprimirCentrado((LINES/2) -2, "Selecciona una opcion");


        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE);
            
            ImprimirCentrado((LINES/2)+i, menu[i]); 
            //mvprintw(5 + i, 10, "%s", menu[i]);
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

int main() {
    menu();
    //Catalogo("mo");
    return 0;
}