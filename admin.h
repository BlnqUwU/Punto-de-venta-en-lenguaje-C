typedef struct user
{
    char nombre[100];
    char apellido[100];
    char correo[100];
    char usr[100];
    char pass[100];
} usuario;

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

//define el contenido de un elemento de la lista
typedef struct informacion {
	
	usuario u;
	
} info;

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

//define el contenido de un elemento de la lista
typedef struct inf {
	
	producto p;
	
} prod;

//guarda el contenido de la lista e indica que elemento sigue y cual esta antes
typedef struct nodop {
	
    prod prod;
    struct nodop * sig;
	struct nodop * ant;
  	
} NODOP;

//indica el numero de elementos en la lista, el inicio y el final de esta
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

//alias para la lista
typedef  cabecera * lista;
typedef  NODO * enlace;
typedef  enum men { NO_MEMORY , OKE , INDEXOUTOFBOUND, EMPTY } mensaje;
typedef  enum b { FALSO , TRU } booleano;

typedef  cabecerap * listaproducto;
typedef  NODOP * enlaceproducto;

typedef cabecera_venta * listaventa;
typedef NODOVENTA * enlaceventa;

int SolicitarSesion(usuario u);
void hash(char *input, char *output);
void ImprimirCentrado(int fila, const char *texto);
void menu();
void iniciarSesion();
void MenuPrincipal(char *usuario);
void AdministrarUsuarios();
void AdministrarCatalogo();
void GenerarReportes();
int RegistrarUsuario(usuario u, char *arch);
int ModificarAtributo(usuario u, char *usr);
int VerificarCorreo(char *correo);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
lista ObtenerUsuarios();
int ModificarAtributoCatalogo(usuario u, char *producto);
listaproducto ObtenerProductos();
int RegistrarProducto(prod u, char *arch);
void AgregarProducto();
int BuscarProducto(char *producto);

//lista doblemente enlazada para usuarios
info get ( int pos , lista l );  
enlace ubicar ( int pos , lista l );                                        
mensaje add ( int pos, info E , lista l );         
mensaje borrar (  int Pos , lista l );                   
booleano empty ( lista l  );
mensaje set ( int Pos ,  info E , lista l);      
mensaje  crearlista( lista * l ); 
void  liberarlista( lista * l );  
void Vaciarlista( lista l ); 

//lista doblemente enlazada para productos

prod getproducto ( int pos , listaproducto l );  
enlaceproducto ubicarproducto ( int pos , listaproducto l );                                        
mensaje addproducto ( int pos, prod E , listaproducto l );         
mensaje borrarproducto (  int Pos , listaproducto l );                   
booleano emptyproducto ( listaproducto l  );
mensaje setproducto ( int Pos ,  prod E , listaproducto l);      
mensaje  crearlistaproducto( listaproducto * l ); 
void  liberarlistaproducto( listaproducto * l );  
void Vaciarlistaproducto( listaproducto l ); 

//lista doblemente enlazada para ventas
infoventa getventa ( int pos , listaventa l );  
enlaceventa ubicarventa ( int pos , listaventa l );                                        
mensaje addventa ( int pos, infoventa E , listaventa l );         
mensaje borrarventa ( int Pos , listaventa l );                   
booleano emptyventa ( listaventa l );
mensaje setventa ( int Pos , infoventa E , listaventa l);      
mensaje crearlistaventa( listaventa * l ); 
void liberarlistaventa( listaventa * l );  
void Vaciarlistaventa( listaventa l ); 