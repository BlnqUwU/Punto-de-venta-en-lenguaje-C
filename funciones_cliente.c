#include "cliente.h"
#include "listas.h"
#include "conexionipc.h"

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

    return 0;//si no coincide el token con correo, retorna 0 y termina
}

//misma logica que BuscarCorreo
int BuscarUsuario(char *usr){

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
    return enviarusuario(u, 0, NULL);
}

//verifica que exista el usuario y que su contraseña sea correcta
int SolicitarSesion(usuario u){

    return solicitarSesion(u);
}

usuario SolicitarPerfil(char *usr){

    usuario u = {"","","","",""};
    strncpy(u.usr, usr, sizeof(u.usr) - 1);
    return obtenerUsuario(u);

}

//modifica un atributo especifico del usuario y guarda los cambios en el archivo correspondiente
int ModificarAtributo(usuario u, char *usr){

    return enviarusuario(u, 2, usr);
}