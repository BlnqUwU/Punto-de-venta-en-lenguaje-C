//se define el tipo de dato usuario
typedef struct user
{
    char nombre[100];
    char apellido[100];
    char correo[100];
    char usr[100];
    char pass[100];
} usuario;

typedef struct informacion {
	
	char producto[100];
    int cantidad;
    float precio;
	
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

typedef  cabecera * lista;
typedef  NODO * enlace;
typedef  enum men { NO_MEMORY , OKE , INDEXOUTOFBOUND, EMPTY } mensaje;
typedef  enum b { FALSO , TRU } booleano;


//prototipos de funciones
int VerificarCorreo(char *correo);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
int BuscarPassword(char *pass);
int RegistrarUsuario(usuario u,char *arch);
int SolicitarSesion(usuario u);
int ComprobarPassword(char *pass);
void hash(char *input, char *output);
void registrar();
void iniciarSesion();
void menu();
void MenuPrincipal(char *usuario);
void Catalogo(char *usuario);
void Carrito(char *usuario);
int Perfil(char *usuario);
void ImprimirCentrado(int fila, const char *texto);
usuario SolicitarPerfil(char *usr);
int ModificarAtributo(usuario u, char *usr);

//lista doblemente enlazada
info get ( int pos , lista l );  
enlace ubicar ( int pos , lista l );                                        
mensaje add ( int pos, info E , lista l );         
mensaje borrar (  int Pos , lista l );                   
booleano empty ( lista l  );
mensaje set ( int Pos ,  info E , lista l);      
mensaje  crearlista( lista * l ); 
void  liberarlista( lista * l );  
void Vaciarlista( lista l );  