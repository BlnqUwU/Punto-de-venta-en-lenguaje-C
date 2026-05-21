#include <ncurses.h>
#include <string.h>
#include "sesion.h"

void hash(char *input, char *output) {
    unsigned long h = 5381; // semilla tipo djb2

    int c;
    while ((c = *input++)) {
        h = ((h << 5) + h) + c; // h * 33 + c
    }

    sprintf(output, "%lu", h);
}

void registrar() {

    initscr();
    set_escdelay(0);
    curs_set(0);
    keypad(stdscr, TRUE);
    usuario user = {"","","","",""};
    clear();
    echo();
    char pass[50], hash_pass[100];
    int opcion = 0;
    int tecla;

    char *menu[] = {
        "Nombre:",
        "Apellido:",
        "Correo:",
        "Usuario:",
        "Password:",
        "Registrar",
        "Regresar"
    };

    int n = 7;
    
    while(1) {

        mvprintw(3, 10, "Registrar un usuario");

        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE); 
            mvprintw(5 + i, 10, "%s", menu[i]);
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
                    move(5,8+9);
                    clrtoeol(); 
                    echo();
                    getstr (user.nombre);
                    noecho();
                    break;
                    case 1:
                    move(6,10+9);
                    clrtoeol(); 
                    echo();
                    getstr (user.apellido);
                    noecho();
                    break;
                    case 2:
                    move(7,8+9);
                    clrtoeol(); 
                    echo();
                    getstr (user.correo);
                    noecho();
                    if (VerificarCorreo(user.correo) == 0) {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
                        clrtoeol();
                        mvprintw(15, 10, "Correo Invalido");
                    }else if (BuscarCorreo(user.correo) == 1) {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
                        clrtoeol();
                        move(7,8+9);
                        clrtoeol();
                        strcpy(user.correo, ""); 
                        mvprintw(15, 10, "El Correo ya esta en uso");
                    }else {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
                        clrtoeol();
                    }
                    break;
                    case 3:
                    echo();
                    move(8,9+9);
                    clrtoeol(); 
                    getstr (user.usr);
                    noecho();
                    if (BuscarUsuario(user.usr) == 1) {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
                        clrtoeol();
                        move(8,9+9);
                        clrtoeol();
                        strcpy(user.usr, ""); 
                        mvprintw(15, 10, "Usuario Existente");
                    }else {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
                        clrtoeol();
                    }
                    break;
                    case 4:
                    noecho();
                    move(9,10+9);
                    clrtoeol(); 
                    getstr(pass);
                    noecho();

                    if (ComprobarPassword(pass) < 5) {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
                        clrtoeol();
                        noecho();
                        mvprintw(15, 10, "La password debe tener minimo 12 caracteres de largo.");
                        noecho();
                        mvprintw(15, 10, "La password debe ser una combinacion de mayusculas, minusculas, numeros y simbolos ( por ejemplo: ! # $ & , - . )");
                        noecho();
                    }else {
                        move(15, 0);
                        clrtoeol();
                        move(16, 0);
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
                            break;
                        }else{
                            if (RegistrarUsuario(user) == 1) {
                                mvprintw(3, 10, "Registro Exitoso");
                                getch();
                                endwin();
                                return;
                            }else {
                                mvprintw(3, 10, "Registro fallido.");
                                getch();
                                endwin();
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

    initscr();
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

        mvprintw(3, 10, "Iniciar Sesion");

        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE); 
            mvprintw(5 + i, 10, "%s", menu[i]);
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
                    move(5,9+9);
                    clrtoeol(); 
                    echo();
                    getstr (user.usr);
                    noecho();
                    break;
                    case 1:
                    move(6,10+9);
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
                        mvprintw(3, 10, "Bienvenido.");
                        getch();
                        return;
                    }
                    mvprintw(10, 10, "Datos incorrectos.");
                    strcpy(user.usr, "");
                    move(5,9+9);
                    clrtoeol();
                    strcpy(user.pass, "");
                    move(6,10+9);
                    clrtoeol(); 
                    errorCount++;
                    if(errorCount == 3) {
                        clear();
                        mvprintw(3, 10, "3 Intentos Fallidos");
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

    while(1) {
        curs_set(0);
        clear();

        mvprintw(3, 10, "Selecciona una opcion:");

        for(int i = 0; i < n; i++) {
            if(i == opcion)
                attron(A_REVERSE); 
            mvprintw(5 + i, 10, "%s", menu[i]);
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
            case 27:
            return;
        }
    }
}

int main() {
    menu();
    return 0;
}