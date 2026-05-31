#include "cliente.h"
#include "listas.h"

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
int BuscarCorreo(char *correo){
    return 0;
}
int BuscarUsuario(char *usr){
    return 0;
}

