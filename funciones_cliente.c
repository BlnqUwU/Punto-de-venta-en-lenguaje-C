#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "cliente.h"

int VerificarCorreo(char *correo){
    for(int i=0; correo[i] != '\0'; i++){ //explora toda la cadena de correo
        if(correo[i]== '@'){ //verifica que exista un @ primero
            for(int j=i; correo[j] != '\0'; j++){ //verifica el resto de la cadena
                if(correo[j]=='.'){ //verifica que exista un . despues del arroba
                return 1; //si existe retorna 1 y termina
                }
            }
        }
    }
    return 0; //si no existe @ ni . entonces retorna 0 y termina
}

int BuscarCorreo(char *correo){
    char linea[100];
    FILE *archivo;
    archivo = fopen("usuarios.txt", "r"); //abre usuarios.txt en modo lectura
    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) { //copia lo que hay en archivo
            char *token = strtok(linea, ",");//elimina todo despues de una coma
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");//se hace hasta quedarse con lo que haya entre la segunda y tercera coma
            while (token != NULL) {
                if (strcmp(token, correo) == 0) {//compara lo que hay en token con lo que hay en puntero
                    fclose(archivo);
                    return 1;//si coincide, cierra el archivo, regresa 1 y termina
                }
                token = strtok(NULL, ",");//va a la siguiente linea en el archivo
            }
        }
        fclose(archivo);
    }
    return 0;//si no coincide el token con correo, retorna 0 y termina
}

//misma logica que BuscarCorreo
int BuscarUsuario(char *usr){
    char linea[100];
    FILE *archivo;
    archivo = fopen("usuarios.txt", "r");
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

int BuscarPassword(char *pass){
    char linea[100];
    FILE *archivo;
    archivo = fopen("usuarios.txt", "r");
    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) {
            char *token = strtok(linea, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            if (strcmp(token, pass) == 0) {
                fclose(archivo);
                return 1;
            }
        }
        fclose(archivo);
    }
    return 0;
}

int ComprobarPassword(char *pass){
    int i = 0;
    int Verificaciones = 0;
    int mayus = 0,minus = 0,num = 0,punt = 0;

    while (pass[i] != '\0') {//pasa por todo el arreglo de password
        i++;//cuenta
    }
    if(i>=12){//verifica si pass tiene una longitud mayor a 12
        Verificaciones++;
    }
    for(int j = 0; pass[j] != '\0'; j++){//pasa por todo el arreglo de password
        if(isupper(pass[j])){//verifica si existe una mayuscula
            mayus=1;
        }else if (islower(pass[j])){//verifica si existe una minuscula
            minus=1;
        }else if(isdigit(pass[j])){//verifica si existe un numero
            num=1;
        }else if(ispunct(pass[j])){//verifica si existe un simbolo
            punt=1;
        }
    }
    if(mayus){
        Verificaciones++;
    }
    if(minus){
        Verificaciones++;
    }
    if(num){
        Verificaciones++;
    }
    if(punt){
        Verificaciones++;
    }

    return Verificaciones;
}

//rescribe los datos ingresados del usuario en un archivo de texto
int RegistrarUsuario(usuario u, char *arch){
    FILE *archivo = fopen(arch, "a");
        if (archivo != NULL) {
            fprintf(archivo, "%s,", u.nombre);
                if(strcmp(u.apellido, "")==0)
                    fprintf(archivo, " ,");
                else
                    fprintf(archivo, "%s,", u.apellido);
            fprintf(archivo, "%s,", u.correo);
            fprintf(archivo, "%s,", u.usr);
            fprintf(archivo, "%s,", u.pass);
            fprintf(archivo, "\n");
            fclose(archivo);
            return 1;//si el registro se hizo retorna 1 y termina
        } else {
            return 0;//si no se pudo crear/abrir el archivo retorna 0 y termina
        }
}

//verifica que exista el usuario y que su contraseña sea correcta
int SolicitarSesion(usuario u){
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {

        char nombre[50], apellido[50], correo[50];
        char usr[50], pass[100];

        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               nombre, apellido, correo, usr, pass);

        if(strcmp(usr, u.usr) == 0 &&
            strcmp(pass, u.pass) == 0) {

            fclose(archivo);
            return 1; // login correcto
        }
    }

    fclose(archivo);
    return 0; // login incorrecto
}

usuario SolicitarPerfil(char *usr){
    char linea[200];
    usuario user = {"No usuario","si encuentras esto","felicidades","rompiste a alguien que siempre sonreia.",""};
    FILE *archivo = fopen("usuarios.txt", "r");

    if (archivo == NULL) 
        return user;

    while (fgets(linea, sizeof(linea), archivo)) {

        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               user.nombre, user.apellido, user.correo, user.usr, user.pass);
        if(strcmp(user.usr, usr) == 0){
            fclose(archivo);
            return user; // login correcto
        }
    }
    return user;
}

int ModificarAtributo(usuario u, char *usr){
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    usuario user = {"","","","",""};

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {

        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               user.nombre, user.apellido, user.correo, user.usr, user.pass);
        if(strcmp(user.usr, usr) == 0){
            //fclose(archivo);
            RegistrarUsuario(u, "temp.txt");
        }else{
            RegistrarUsuario(user, "temp.txt");
        }
        
    }
     fclose(archivo);
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
        return 1;   
}

void hash(char *input, char *output) {
    unsigned long h = 5381; // semilla tipo djb2

    int c;
    while ((c = *input++)) {
        h = ((h << 5) + h) + c; // h * 33 + c
    }

    sprintf(output, "%lu", h);
}

void ImprimirCentrado(int fila, const char *texto) {
    int col = (COLS - strlen(texto)) / 2;
    if (col < 0) col = 0;
    mvprintw(fila, col, "%s", texto);
}