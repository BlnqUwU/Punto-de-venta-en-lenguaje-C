#include <ncurses.h>
#include <string.h>
#include "cliente.h"


void Catalogo(usuario u){
    return;
}

void Carrito(usuario u){
    return;
}

void Perfil(usuario u){
    return;
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
        curs_set(0);
        clear();
        
        mvprintw((LINES/2)-3, ((COLS-strlen("!Bienvenido ")-strlen(u.usr))/2), "!Bienvenido %s!", u.usr);
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
                        Catalogo(u);
                        break;
                    case 1:
                        Carrito(u);
                        break;
                    case 2:
                        Perfil(u);
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
    char *menu[] = {
        "Nombre:",
        "Apellido:",
        "Correo:",
        "Usuario:",
        "Password:",
        "Registrar",
        "Regresar "
    };

    int n = 7;
    
    while(1) {
        //curs_set(0);
        //clear();

        ImprimirCentrado((LINES/2) -3, "Registrar un usuario");
        //mvprintw(3, 10, "Registrar un usuario");

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
                    move((LINES/2)+2,8+(COLS/2)-26);
                    clrtoeol(); 
                    echo();
                    getstr (user.correo);
                    noecho();
                    if (VerificarCorreo(user.correo) == 0) {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                        ImprimirCentrado((LINES/2)+11, "Correo Invalido");
                        //mvprintw(15, 10, "Correo Invalido");
                    }else if (BuscarCorreo(user.correo) == 1) {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                        move((LINES/2)+7,8+49);
                        clrtoeol();
                        mvprintw((LINES/2)+11, ((COLS-strlen("El correo  ya esta en uso")-strlen(user.correo))/2), "El Correo %s ya esta en uso", user.correo);
                        strcpy(user.correo, "");
                    }else {
                        move((LINES/2)+11, 0);
                        clrtoeol();
                        move((LINES/2)+12, 0);
                        clrtoeol();
                    }
                    break;
                    case 3:
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
                        col= (COLS - strlen(user.usr)) / 2;
                        if(col<0)
                            col=0;
                        mvprintw((LINES/2)+11, col, "El usuario %s ya existe", user.usr);
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
                            if (RegistrarUsuario(user) == 1) {
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
    usuario user;

    char pass[50], hash_pass[50];

    int opcion = 0;
    int tecla;



    char *menu[] = {
        "Usuario:",
        "Password:",
        "Entrar",
        "Regresar"
    };

    int n = 4;
    
    while(errorCount != 3) {
        
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

                    if(SolicitarSesion(user) == 1) {
                        clear();
                        MenuPrincipal(user);
                        return;
                    }
                    
                    ImprimirCentrado((LINES/2) +11, "Usuario y/o password incorrectos.");
                    //mvprintw(10, 10, "Datos incorrectos.");
                    strcpy(user.usr, "");
                    move(5,9+(COLS/2) -26);
                    clrtoeol();
                    strcpy(user.pass, "");
                    move(6,10+(COLS/2) -26);
                    clrtoeol(); 
                    errorCount++;
                    if(errorCount == 3) {
                        clear();
                        ImprimirCentrado((LINES/2), "3 intentos fallidos.");
                        //mvprintw(3, 10, "3 Intentos Fallidos");
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

    initscr();
    set_escdelay(0);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); 

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_CYAN);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        init_pair(3, COLOR_WHITE, COLOR_GREEN);
        init_pair(4, COLOR_WHITE, COLOR_RED);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
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
    return 0;
}