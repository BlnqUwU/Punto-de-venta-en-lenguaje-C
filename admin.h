#ifndef ADMIN_H
#define ADMIN_H

typedef struct user
{
    char nombre[100];
    char apellido[100];
    char correo[100];
    char usr[100];
    char pass[100];
} admin;

typedef struct product
{
    char producto[100];
    int cantidad;
    float precio;
} producto;

typedef struct ventas
{
    char fecha[100];
    float total;
} venta;

typedef struct informacion {
    admin a;
} info;

typedef struct nodo {
    info Inf;
    struct nodo * sig;
    struct nodo * ant;
} NODO;

typedef struct Cab {
    NODO * I;
    NODO * F;
    int NE;
} cabecera;

typedef struct inf {
    producto p;
} prod;

typedef struct nodop {
    prod prod;
    struct nodop * sig;
    struct nodop * ant;
} NODOP;

typedef struct Cabp {
    NODOP * I;
    NODOP * F;
    int NE;
} cabecerap;

typedef struct informacionventa {
    venta v;
} infoventa;

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

typedef  cabecera * lista;
typedef  NODO * enlace;
typedef  enum men { NO_MEMORY , OKE , INDEXOUTOFBOUND, EMPTY } mensaje;
typedef  enum b { FALSO , TRU } booleano;

typedef  cabecerap * listaproducto;
typedef  NODOP * enlaceproducto;

typedef cabecera_venta * listaventa;
typedef NODOVENTA * enlaceventa;

// ──────────────────────────────────────────
// PROTOTIPOS — LOGIN ADMIN
// ──────────────────────────────────────────

int  buscarAdmin(char *usr);
int  registrarAdmin(admin a);
int  solicitarSesionAdmin(admin a);
void crearAdminSiNoExiste();
void hash(char *input, char *output);

// ──────────────────────────────────────────
// PROTOTIPOS — FUNCIONES
// ──────────────────────────────────────────

int SolicitarSesion(admin a);
void ImprimirCentrado(int fila, const char *texto);
void cargarVentasPorRango(listaventa lv, int dias);
void menu();
void iniciarSesion();
void MenuPrincipal(char *usuario);
void AdministrarUsuarios();
void AdministrarCatalogo();
void GenerarReportes();
int RegistrarUsuario(admin a, char *arch);
int ModificarAtributo(admin a, char *usr);
int ModificarAtributoCatalogo(admin u, char *producto);
int VerificarCorreo(char *correo);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
lista ObtenerUsuarios();
listaproducto ObtenerProductos();
int RegistrarProducto(prod u, char *arch);
void AgregarProducto();
int BuscarProducto(char *producto);

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

prod getproducto(int pos, listaproducto l);
enlaceproducto ubicarproducto(int pos, listaproducto l);
mensaje addproducto(int pos, prod E, listaproducto l);
mensaje borrarproducto(int Pos, listaproducto l);
booleano emptyproducto(listaproducto l);
mensaje setproducto(int Pos, prod E, listaproducto l);
mensaje crearlistaproducto(listaproducto *l);
void liberarlistaproducto(listaproducto *l);
void Vaciarlistaproducto(listaproducto l);

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