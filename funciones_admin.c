#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "admin.h"

int SolicitarSesion(usuario u){
    char linea[200];
    FILE *archivo = fopen("admin.txt", "r");

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

int RegistrarUsuario(usuario u, char *arch){
    FILE *archivo = fopen(arch, "a");
        if (archivo != NULL) {
            if(strcmp(u.nombre, "")==0)
                fprintf(archivo, " ,");
            else
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

int ModificarAtributo(usuario u, char *usr){
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    usuario user = {"","","","",""};

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {

        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               user.nombre, user.apellido, user.correo, user.usr, user.pass);
        if(strcmp(user.usr, usr) == 0){
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

int ModificarAtributoCatalogo(usuario u, char *usr){
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    usuario user = {"","","","",""};

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {

        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               user.nombre, user.apellido, user.correo, user.usr, user.pass);
        if(strcmp(user.usr, usr) == 0){
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

lista ObtenerUsuarios(){
    char linea[200];
    FILE *archivo = fopen("usuarios.txt", "r");
    lista usuarios;
    crearlista(&usuarios);
    usuario user = {"","","","",""};
    info usr;
    int cont=0;

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {

        sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,\n]",
               user.nombre, user.apellido, user.correo, user.usr, user.pass);
        usr.u=user;
        add(cont,usr,usuarios);
        cont++;
    }
    fclose(archivo);
    return usuarios;
}

listaproducto ObtenerProductos(){
    char linea[200];
    FILE *archivo = fopen("catalogo.txt", "r");
    listaproducto productos;
    crearlistaproducto(&productos);
    producto producto = {"",0,0};
    prod fin;
    int cont=0;
    char cantidad[50];
    char precio[50];

    if (archivo == NULL) return 0;

    while (fgets(linea, sizeof(linea), archivo)) {

        sscanf(linea, "%[^,],%[^,],%[^,\n]",
               producto.producto,cantidad,precio);
        producto.cantidad=atoi(cantidad);
        producto.precio=atof(precio);
        fin.p=producto;
        addproducto(cont,fin,productos);
        cont++;
    }
    fclose(archivo);
    return productos;
}

int RegistrarProducto(prod u, char *arch){
    FILE *archivo = fopen(arch, "a");
        if (archivo != NULL) {
            fprintf(archivo, "%s,", u.p.producto);
            fprintf(archivo, "%d,", u.p.cantidad);
            fprintf(archivo, "%f,", u.p.precio);
            fprintf(archivo, "\n");
            fclose(archivo);
            return 1;//si el registro se hizo retorna 1 y termina
        } else {
            return 0;//si no se pudo crear/abrir el archivo retorna 0 y termina
        }
}

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
        return 0;
    }
    return 0;
}

int BuscarProducto(char *producto){
    char linea[100];
    FILE *archivo;
    archivo = fopen("catalogo.txt", "r");
    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) {
            char *token = strtok(linea, ",");
            while (token != NULL) {
                if (strcmp(token, producto) == 0) {
                    fclose(archivo);
                    return 1;
                }
                token = strtok(NULL, ",");
            }
        }
        fclose(archivo);
        return 0;
    }
    return 0;
}

//lista doblemente enlazada

info get ( int pos , lista l ) {
	
	info copia;
	
	enlace Ite= ubicar (pos , l);
	
	if (Ite == NULL)
	    return copia;
	
	copia=Ite->Inf;
	
	return copia;   
}                                        

enlace ubicar ( int p , lista l ){
	int mitad,i;
	enlace Ite;
	if (l == NULL || empty (l))
	    return NULL;    
	if (p>=0 && p<=l->NE-1)    
	    {
	       mitad=l->NE/2;
	       
	       if ( p <= mitad )
	        {
	        	Ite=l->I;
	        	
	        	for(i=0;i<p;i++)
	        	    Ite=Ite->sig;
						  
			} else {
				
				Ite=l->F;
	        	
	        	for(i=0;i<(l->NE-1)-p;i++)
	        	    Ite=Ite->ant;	    
			}
	       
	       return Ite;
		}
	return NULL;	
} 


mensaje add ( int pos, info E , lista l ){
	
	enlace act,ant;
	
	enlace temp = (enlace) malloc (sizeof(NODO));
	
	if (temp == NULL)
	    return NO_MEMORY; 
	
	temp->Inf=E;
		
	if ( empty (l) )
	   {
	   	  temp-> ant = temp -> sig = NULL;
		  l->I=l->F=temp;	    
	   } else {
	   	
	   	  if (pos <= 0){ // Al Inicio
	   	  	    temp->sig=l->I;
	   	  	    temp->ant=NULL;
	   	  	    l->I->ant=temp;
	   	  	    l->I=temp;
			 }else if( pos >= l->NE){ // Al final
			 	temp->sig=NULL;
	   	  	    temp->ant=l->F;
	   	  	    l->F->sig=temp;
			    l->F=temp;
			 }else{ // En medio
		
		        act= ubicar (pos , l);
				ant= act->ant;
				     	 	
			 	temp->sig=act;
			 	temp->ant=ant;
			 	act->ant=temp;
			 	ant->sig=temp;
			 	
			 }
	   }
	   
	l->NE++;
	return OKE;
}        

mensaje borrar (  int p , lista l ){

    enlace temp,s,a,act;
    
	if (l == NULL || empty (l))
	    return EMPTY;
	    
	if ( !(p>=0 && p<l->NE) )    
		return INDEXOUTOFBOUND;

    if( p == 0 && l->NE == 1 ) // lista con un elemento
    {
    	 free(l->I);
    	 l->I=l->F=NULL;
    	 
	}else if (p == 0 && l->NE > 1){  // Al Inicio
		
		 temp=l->I->sig;
		 free(l->I);
		 temp->ant=NULL;
		 l->I=temp;
		 
	} else if ( p == l->NE-1 ){ // Al final
		
		  temp=l->F->ant;
		  free(l->F);
		  temp->sig=NULL;
		  l->F=temp;
		
	}else {  //En medio
		
		act= ubicar (p , l);
		a=act->ant;
		s=act->sig;
		free(act);
		a->sig=s;
		s->ant=a;
	}
      
    l->NE--;
	return OKE;   

}                

booleano empty ( lista l  ){
	
    if ( l != NULL && l->I == NULL && l->F == NULL && l->NE == 0 )	
	    return TRU;
	return FALSO;
}

mensaje set ( int p , info E , lista l){
	enlace Ite;
	if (l == NULL || empty (l))
	    return EMPTY;
	    
	if ( !(p>=0 && p<l->NE) )    
		return INDEXOUTOFBOUND;
		
	Ite=ubicar (p , l);	
		  
	Ite->Inf=E;
	
	return OKE;
}      

mensaje crearlista( lista * l ){
	
	lista temp= (lista) malloc (sizeof(cabecera));
	
	if ( temp == NULL)
	    return NO_MEMORY;
	
	temp->I=temp->F=NULL;
	temp->NE=0;
	
	*l=temp;
	
	return OKE;
}

void  liberarlista( lista * l ){
	if ( *l != NULL ){
	
	  Vaciarlista ( *l  );
	  
	  free(*l);
	  
	  *l=NULL;
    }
}  

void  Vaciarlista( lista l ){
	
	   while (!empty (l)){
	   	
	   	     borrar(0,l);
	   	
	   }
	
}   

prod getproducto ( int pos , listaproducto l ) {
	
	prod copia;
	
	enlaceproducto Ite= ubicarproducto(pos , l);
	
	if (Ite == NULL)
	    return copia;
	
	copia=Ite->prod;
	
	return copia;   
}                                        

enlaceproducto ubicarproducto ( int p , listaproducto l ){
	int mitad,i;
	enlaceproducto Ite;
	if (l == NULL || emptyproducto (l))
	    return NULL;    
	if (p>=0 && p<=l->NE-1)    
	    {
	       mitad=l->NE/2;
	       
	       if ( p <= mitad )
	        {
	        	Ite=l->I;
	        	
	        	for(i=0;i<p;i++)
	        	    Ite=Ite->sig;
						  
			} else {
				
				Ite=l->F;
	        	
	        	for(i=0;i<(l->NE-1)-p;i++)
	        	    Ite=Ite->ant;	    
			}
	       
	       return Ite;
		}
	return NULL;	
} 


mensaje addproducto ( int pos, prod E , listaproducto l ){
	
	enlaceproducto act,ant;
	
	enlaceproducto temp = (enlaceproducto) malloc (sizeof(NODOP));
	
	if (temp == NULL)
	    return NO_MEMORY; 
	
	temp->prod=E;
		
	if ( emptyproducto (l) )
	   {
	   	  temp-> ant = temp -> sig = NULL;
		  l->I=l->F=temp;	    
	   } else {
	   	
	   	  if (pos <= 0){ // Al Inicio
	   	  	    temp->sig=l->I;
	   	  	    temp->ant=NULL;
	   	  	    l->I->ant=temp;
	   	  	    l->I=temp;
			 }else if( pos >= l->NE){ // Al final
			 	temp->sig=NULL;
	   	  	    temp->ant=l->F;
	   	  	    l->F->sig=temp;
			    l->F=temp;
			 }else{ // En medio
		
		        act= ubicarproducto (pos , l);
				ant= act->ant;
				     	 	
			 	temp->sig=act;
			 	temp->ant=ant;
			 	act->ant=temp;
			 	ant->sig=temp;
			 	
			 }
	   }
	   
	l->NE++;
	return OKE;
}        

mensaje borrarproducto (  int p , listaproducto l ){

    enlaceproducto temp,s,a,act;
    
	if (l == NULL || emptyproducto (l))
	    return EMPTY;
	    
	if ( !(p>=0 && p<l->NE) )    
		return INDEXOUTOFBOUND;

    if( p == 0 && l->NE == 1 ) // listaproducto con un elemento
    {
    	 free(l->I);
    	 l->I=l->F=NULL;
    	 
	}else if (p == 0 && l->NE > 1){  // Al Inicio
		
		 temp=l->I->sig;
		 free(l->I);
		 temp->ant=NULL;
		 l->I=temp;
		 
	} else if ( p == l->NE-1 ){ // Al final
		
		  temp=l->F->ant;
		  free(l->F);
		  temp->sig=NULL;
		  l->F=temp;
		
	}else {  //En medio
		
		act= ubicarproducto (p , l);
		a=act->ant;
		s=act->sig;
		free(act);
		a->sig=s;
		s->ant=a;
	}
      
    l->NE--;
	return OKE;   

}                

booleano emptyproducto ( listaproducto l  ){
	
    if ( l != NULL && l->I == NULL && l->F == NULL && l->NE == 0 )	
	    return TRU;
	return FALSO;
}

mensaje setproducto ( int p , prod E , listaproducto l){
	enlaceproducto Ite;
	if (l == NULL || emptyproducto (l))
	    return EMPTY;
	    
	if ( !(p>=0 && p<l->NE) )    
		return INDEXOUTOFBOUND;
		
	Ite=ubicarproducto (p , l);	
		  
	Ite->prod=E;
	
	return OKE;
}      

mensaje crearlistaproducto( listaproducto * l ){
	
	listaproducto temp= (listaproducto) malloc (sizeof(cabecera));
	
	if ( temp == NULL)
	    return NO_MEMORY;
	
	temp->I=temp->F=NULL;
	temp->NE=0;
	
	*l=temp;
	
	return OKE;
}

void  liberarlistaproducto( listaproducto * l ){
	if ( *l != NULL ){
	
	  Vaciarlistaproducto ( *l  );
	  
	  free(*l);
	  
	  *l=NULL;
    }
}  

void  Vaciarlistaproducto( listaproducto l ){
	
	   while (!emptyproducto (l)){
	   	
	   	     borrarproducto(0,l);
	   	
	   }
	
}   


infoventa getventa ( int pos , listaventa l ) {
    infoventa copia;
    enlaceventa Ite = ubicarventa(pos, l);
    if (Ite == NULL)
        return copia;
    copia = Ite->Inf;
    return copia;   
}                                        

enlaceventa ubicarventa ( int p , listaventa l ) {
    int mitad, i;
    enlaceventa Ite;
    if (l == NULL || emptyventa(l))
        return NULL;    
    if (p >= 0 && p <= l->NE-1) {
        mitad = l->NE / 2;
        if (p <= mitad) {
            Ite = l->I;
            for (i = 0; i < p; i++)
                Ite = Ite->sig;
        } else {
            Ite = l->F;
            for (i = 0; i < (l->NE-1)-p; i++)
                Ite = Ite->ant;       
        }
        return Ite;
    }
    return NULL;    
} 

mensaje addventa ( int pos, infoventa E , listaventa l ) {
    enlaceventa act, ant;
    enlaceventa temp = (enlaceventa) malloc(sizeof(NODOVENTA));
    if (temp == NULL)
        return NO_MEMORY; 
    temp->Inf = E;
    if (emptyventa(l)) {
        temp->ant = temp->sig = NULL;
        l->I = l->F = temp;       
    } else {
        if (pos <= 0) {
            temp->sig = l->I;
            temp->ant = NULL;
            l->I->ant = temp;
            l->I = temp;
        } else if (pos >= l->NE) {
            temp->sig = NULL;
            temp->ant = l->F;
            l->F->sig = temp;
            l->F = temp;
        } else {
            act = ubicarventa(pos, l);
            ant = act->ant;
            temp->sig = act;
            temp->ant = ant;
            act->ant = temp;
            ant->sig = temp;
        }
    }
    l->NE++;
    return OKE;
}        

mensaje borrarventa ( int p , listaventa l ) {
    enlaceventa temp, s, a, act;
    if (l == NULL || emptyventa(l))
        return EMPTY;
    if (!(p >= 0 && p < l->NE))    
        return INDEXOUTOFBOUND;
    if (p == 0 && l->NE == 1) {
        free(l->I);
        l->I = l->F = NULL;
    } else if (p == 0 && l->NE > 1) {
        temp = l->I->sig;
        free(l->I);
        temp->ant = NULL;
        l->I = temp;
    } else if (p == l->NE-1) {
        temp = l->F->ant;
        free(l->F);
        temp->sig = NULL;
        l->F = temp;
    } else {
        act = ubicarventa(p, l);
        a = act->ant;
        s = act->sig;
        free(act);
        a->sig = s;
        s->ant = a;
    }
    l->NE--;
    return OKE;   
}                

booleano emptyventa ( listaventa l ) {
    if (l != NULL && l->I == NULL && l->F == NULL && l->NE == 0)  
        return TRU;
    return FALSO;
}

mensaje setventa ( int p , infoventa E , listaventa l ) {
    enlaceventa Ite;
    if (l == NULL || emptyventa(l))
        return EMPTY;
    if (!(p >= 0 && p < l->NE))    
        return INDEXOUTOFBOUND;
    Ite = ubicarventa(p, l); 
    Ite->Inf = E;
    return OKE;
}      

mensaje crearlistaventa( listaventa * l ) {
    listaventa temp = (listaventa) malloc(sizeof(cabecera_venta));
    if (temp == NULL)
        return NO_MEMORY;
    temp->I = temp->F = NULL;
    temp->NE = 0;
    *l = temp;
    return OKE;
}

void liberarlistaventa( listaventa * l ) {
    if (*l != NULL) {
        Vaciarlistaventa(*l);
        free(*l);
        *l = NULL;
    }
}  

void Vaciarlistaventa( listaventa l ) {
    while (!emptyventa(l)) {
        borrarventa(0, l);
    }
}