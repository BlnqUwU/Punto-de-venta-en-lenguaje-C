//se define el tipo de dato usuario
typedef struct user
{
    char nombre[100];
    char apellido[100];
    char correo[100];
    char usr[100];
    char pass[100];
} usuario;

//prototipos de funciones
int VerificarCorreo(char *correo);
int BuscarCorreo(char *correo);
int BuscarUsuario(char *usr);
int BuscarPassword(char *pass);
int RegistrarUsuario(usuario u);
int SolicitarSesion(usuario u);
int ComprobarPassword(char *pass);
void hash(char *input, char *output);
void registrar();
void iniciarSesion();
void menu();