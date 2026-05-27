#ifndef CLIENTE_H
#define CLIENTE_H
#include "utilidades.h"

//se define el tipo de dato usuario
typedef struct user
{
    char nombre[100];
    char apellido[100];
    char correo[100];
    char usr[100];
    char pass[100];
} usuario;

//define el contenido de un elemento de la lista
typedef struct informacionarticulo {

    char producto[100];
    int cantidad;
    float precio;

} infoarticulo;

//guarda el contenido de la lista e indica que elemento sigue y cual esta antes

typedef struct nodoarticulo {
    infoarticulo Inf;
    struct nodoarticulo * sig;
    struct nodoarticulo * ant;
} NODOARTICULO;

//indica el numero de elementos en la lista, el inicio y el final de esta

typedef struct Cabarticulo {
    NODOARTICULO * I;
    NODOARTICULO * F;
    int NE;
} cabecera_articulo;

typedef struct memoriacompartida{
    usuario u;
} usuarioShm;

//alias para la lista

typedef cabecera_articulo * listaarticulo;
typedef NODOARTICULO * enlacearticulo;



//prototipos de funciones
int VerificarCorreo(char *correo);
//int BuscarCorreo(char *correo);
//int BuscarUsuario(char *usr);
//int RegistrarUsuario(usuario u,char *arch);
//int SolicitarSesion(usuario u);
//int ComprobarPassword(char *pass);
void hash(char *input, char *output);
void registrar();
void iniciarSesion();
void menu();
void MenuPrincipal(char *usuario);
void Catalogo(char *usuario);
void Carrito(char *usuario);
int Perfil(char *usuario);
void ImprimirCentrado(int fila, const char *texto);
//usuario SolicitarPerfil(char *usr);
//int ModificarAtributo(usuario u, char *usr);

//lista doblemente enlazada
infoarticulo getarticulo ( int pos , listaarticulo l );  
enlacearticulo ubicararticulo ( int pos , listaarticulo l );                                        
mensaje addarticulo ( int pos, infoarticulo E , listaarticulo l );         
mensaje borrararticulo ( int Pos , listaarticulo l );                   
booleano emptyarticulo ( listaarticulo l );
mensaje setarticulo ( int Pos , infoarticulo E , listaarticulo l);      
mensaje crearlistaarticulo( listaarticulo * l ); 
void liberarlistaarticulo( listaarticulo * l );  
void Vaciarlistaarticulo( listaarticulo l ); 
#endif
