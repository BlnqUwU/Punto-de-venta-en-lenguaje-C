#ifndef LISTAS_H
#define LISTAS_H
#include "utilidades.h"
#undef I
#undef F

//_____________________________________________________________________________________________

//                                          abstracciones

//_____________________________________________________________________________________________

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

} articulo;

typedef struct {
    float total;
    usuario u;
    //char *fecha; // DD/MM/YYYY HH:MM
    char  fecha[20]; //  DD/MM/YYYY HH:MM
} venta;

typedef struct informacion {
    usuario u;
} info;

typedef struct informacionventa {
    venta v;
} infoventa;


//_____________________________________________________________________________________________

//                                          nodos y cabeceras

//_____________________________________________________________________________________________

//*************************************************usuarios***********************************

//guarda el contenido de la lista e indica que elemento sigue y cual esta antes
typedef struct nodo {
    info Inf;
    struct nodo * sig;
    struct nodo * ant;
} NODO;

//indica el numero de elementos en la lista, el inicio y el final de esta
typedef struct Cab {
    NODO * I;
    NODO * F;
    int NE;
} cabecera;

//*************************************************articulos***********************************

typedef struct nodoarticulo {
    articulo Inf;
    struct nodoarticulo * sig;
    struct nodoarticulo * ant;
} NODOARTICULO;

typedef struct Cabarticulo {
    NODOARTICULO * I;
    NODOARTICULO * F;
    int NE;
} cabecera_articulo;

//*************************************************ventas***********************************

typedef struct nodoventa {
    infoventa Inf;
    struct nodoventa * sig;
    struct nodoventa * ant;
} NODOVENTA;

typedef struct Cabventa {
    NODOVENTA * I;
    NODOVENTA * F;
    int NE;
} cabecera_venta;


//_____________________________________________________________________________________________

//                                          alias

//_____________________________________________________________________________________________

//alias para la lista

typedef cabecera_articulo * listaarticulo;
typedef NODOARTICULO * enlacearticulo;

typedef  cabecera * lista;
typedef  NODO * enlace;

typedef cabecera_venta * listaventa;
typedef NODOVENTA * enlaceventa;


//_____________________________________________________________________________________________

//                                          prototipos

//_____________________________________________________________________________________________

//lista doblemente enlazada


// ──────────────────────────────────────────
// PROTOTIPOS — LISTA USUARIOS
// ──────────────────────────────────────────

info get(int pos, lista l);
enlace ubicar(int pos, lista l);
mensaje add(int pos, info E, lista l);
mensaje borrar(int Pos, lista l);
booleano empty(lista l);
mensaje set(int Pos, info E, lista l);
mensaje crearlista(lista *l);
void liberarlista(lista *l);
void Vaciarlista(lista l);

// ──────────────────────────────────────────
// PROTOTIPOS — LISTA PRODUCTOS
// ──────────────────────────────────────────

articulo getarticulo ( int pos , listaarticulo l );  
enlacearticulo ubicararticulo ( int pos , listaarticulo l );                                        
mensaje addarticulo ( int pos, articulo E , listaarticulo l );         
mensaje borrararticulo ( int Pos , listaarticulo l );                   
booleano emptyarticulo ( listaarticulo l );
mensaje setarticulo ( int Pos , articulo E , listaarticulo l);      
mensaje crearlistaarticulo( listaarticulo * l ); 
void liberarlistaarticulo( listaarticulo * l );  
void Vaciarlistaarticulo( listaarticulo l ); 

// ──────────────────────────────────────────
// PROTOTIPOS — LISTA VENTAS
// ──────────────────────────────────────────

infoventa getventa(int pos, listaventa l);
enlaceventa ubicarventa(int pos, listaventa l);
mensaje addventa(int pos, infoventa E, listaventa l);
mensaje borrarventa(int Pos, listaventa l);
booleano emptyventa(listaventa l);
mensaje setventa(int Pos, infoventa E, listaventa l);
mensaje crearlistaventa(listaventa *l);
void liberarlistaventa(listaventa *l);
void Vaciarlistaventa(listaventa l);


#endif