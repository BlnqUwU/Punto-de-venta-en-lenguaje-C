#include "listas.h"

//*************************************************articulos***********************************

articulo getarticulo ( int pos , listaarticulo l ) {
    articulo copia;
    enlacearticulo Ite = ubicararticulo(pos, l);
    if (Ite == NULL)
        return copia;
    copia = Ite->Inf;
    return copia;   
}                                        

enlacearticulo ubicararticulo ( int p , listaarticulo l ) {
    int mitad, i;
    enlacearticulo Ite;
    if (l == NULL || emptyarticulo(l))
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

mensaje addarticulo ( int pos, articulo E , listaarticulo l ) {
    enlacearticulo act, ant;
    enlacearticulo temp = (enlacearticulo) malloc(sizeof(NODOARTICULO));
    if (temp == NULL)
        return NO_MEMORY; 
    temp->Inf = E;
    if (emptyarticulo(l)) {
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
            act = ubicararticulo(pos, l);
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

mensaje borrararticulo ( int p , listaarticulo l ) {
    enlacearticulo temp, s, a, act;
    if (l == NULL || emptyarticulo(l))
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
        act = ubicararticulo(p, l);
        a = act->ant;
        s = act->sig;
        free(act);
        a->sig = s;
        s->ant = a;
    }
    l->NE--;
    return OKE;   
}                

booleano emptyarticulo ( listaarticulo l ) {
    if (l != NULL && l->I == NULL && l->F == NULL && l->NE == 0)  
        return TRU;
    return FALSO;
}

mensaje setarticulo ( int p , articulo E , listaarticulo l ) {
    enlacearticulo Ite;
    if (l == NULL || emptyarticulo(l))
        return EMPTY;
    if (!(p >= 0 && p < l->NE))    
        return INDEXOUTOFBOUND;
    Ite = ubicararticulo(p, l); 
    Ite->Inf = E;
    return OKE;
}      

mensaje crearlistaarticulo( listaarticulo * l ) {
    listaarticulo temp = (listaarticulo) malloc(sizeof(cabecera_articulo));
    if (temp == NULL)
        return NO_MEMORY;
    temp->I = temp->F = NULL;
    temp->NE = 0;
    *l = temp;
    return OKE;
}

void liberarlistaarticulo( listaarticulo * l ) {
    if (*l != NULL) {
        Vaciarlistaarticulo(*l);
        free(*l);
        *l = NULL;
    }
}  

void Vaciarlistaarticulo( listaarticulo l ) {
    while (!emptyarticulo(l)) {
        borrararticulo(0, l);
    }
}

//*************************************************usuarios***********************************

info get(int pos, lista l) {
    info copia;
    enlace Ite = ubicar(pos, l);
    if (Ite == NULL) return copia;
    copia = Ite->Inf;
    return copia;
}

enlace ubicar(int p, lista l) {
    int mitad, i;
    enlace Ite;
    if (l == NULL || empty(l)) return NULL;
    if (p >= 0 && p <= l->NE-1) {
        mitad = l->NE / 2;
        if (p <= mitad) {
            Ite = l->I;
            for (i = 0; i < p; i++) Ite = Ite->sig;
        } else {
            Ite = l->F;
            for (i = 0; i < (l->NE-1)-p; i++) Ite = Ite->ant;
        }
        return Ite;
    }
    return NULL;
}

mensaje add(int pos, info E, lista l) {
    enlace act, ant;
    enlace temp = (enlace) malloc(sizeof(NODO));
    if (temp == NULL) return NO_MEMORY;
    temp->Inf = E;
    if (empty(l)) {
        temp->ant = temp->sig = NULL;
        l->I = l->F = temp;
    } else {
        if (pos <= 0) {
            temp->sig = l->I; temp->ant = NULL;
            l->I->ant = temp; l->I = temp;
        } else if (pos >= l->NE) {
            temp->sig = NULL; temp->ant = l->F;
            l->F->sig = temp; l->F = temp;
        } else {
            act = ubicar(pos, l); ant = act->ant;
            temp->sig = act; temp->ant = ant;
            act->ant = temp; ant->sig = temp;
        }
    }
    l->NE++;
    return OKE;
}

mensaje borrar(int p, lista l) {
    enlace temp, s, a, act;
    if (l == NULL || empty(l)) return EMPTY;
    if (!(p >= 0 && p < l->NE)) return INDEXOUTOFBOUND;
    if (p == 0 && l->NE == 1) {
        free(l->I); l->I = l->F = NULL;
    } else if (p == 0 && l->NE > 1) {
        temp = l->I->sig; free(l->I);
        temp->ant = NULL; l->I = temp;
    } else if (p == l->NE-1) {
        temp = l->F->ant; free(l->F);
        temp->sig = NULL; l->F = temp;
    } else {
        act = ubicar(p, l); a = act->ant; s = act->sig;
        free(act); a->sig = s; s->ant = a;
    }
    l->NE--;
    return OKE;
}

booleano empty(lista l) {
    if (l != NULL && l->I == NULL && l->F == NULL && l->NE == 0) return TRU;
    return FALSO;
}

mensaje set(int p, info E, lista l) {
    enlace Ite;
    if (l == NULL || empty(l)) return EMPTY;
    if (!(p >= 0 && p < l->NE)) return INDEXOUTOFBOUND;
    Ite = ubicar(p, l);
    Ite->Inf = E;
    return OKE;
}

mensaje crearlista(lista *l) {
    lista temp = (lista) malloc(sizeof(cabecera));
    if (temp == NULL) return NO_MEMORY;
    temp->I = temp->F = NULL;
    temp->NE = 0;
    *l = temp;
    return OKE;
}

void liberarlista(lista *l) {
    if (*l != NULL) {
        Vaciarlista(*l);
        free(*l);
        *l = NULL;
    }
}

void Vaciarlista(lista l) {
    while (!empty(l)) borrar(0, l);
}

//*************************************************ventas***********************************

infoventa getventa(int pos, listaventa l) {
    infoventa copia;
    enlaceventa Ite = ubicarventa(pos, l);
    if (Ite == NULL) return copia;
    copia = Ite->Inf;
    return copia;
}

enlaceventa ubicarventa(int p, listaventa l) {
    int mitad, i;
    enlaceventa Ite;
    if (l == NULL || emptyventa(l)) return NULL;
    if (p >= 0 && p <= l->NE-1) {
        mitad = l->NE / 2;
        if (p <= mitad) {
            Ite = l->I;
            for (i = 0; i < p; i++) Ite = Ite->sig;
        } else {
            Ite = l->F;
            for (i = 0; i < (l->NE-1)-p; i++) Ite = Ite->ant;
        }
        return Ite;
    }
    return NULL;
}

mensaje addventa(int pos, infoventa E, listaventa l) {
    enlaceventa act, ant;
    enlaceventa temp = (enlaceventa) malloc(sizeof(NODOVENTA));
    if (temp == NULL) return NO_MEMORY;
    temp->Inf = E;
    if (emptyventa(l)) {
        temp->ant = temp->sig = NULL;
        l->I = l->F = temp;
    } else {
        if (pos <= 0) {
            temp->sig = l->I; temp->ant = NULL;
            l->I->ant = temp; l->I = temp;
        } else if (pos >= l->NE) {
            temp->sig = NULL; temp->ant = l->F;
            l->F->sig = temp; l->F = temp;
        } else {
            act = ubicarventa(pos, l); ant = act->ant;
            temp->sig = act; temp->ant = ant;
            act->ant = temp; ant->sig = temp;
        }
    }
    l->NE++;
    return OKE;
}

mensaje borrarventa(int p, listaventa l) {
    enlaceventa temp, s, a, act;
    if (l == NULL || emptyventa(l)) return EMPTY;
    if (!(p >= 0 && p < l->NE)) return INDEXOUTOFBOUND;
    if (p == 0 && l->NE == 1) {
        free(l->I); l->I = l->F = NULL;
    } else if (p == 0 && l->NE > 1) {
        temp = l->I->sig; free(l->I);
        temp->ant = NULL; l->I = temp;
    } else if (p == l->NE-1) {
        temp = l->F->ant; free(l->F);
        temp->sig = NULL; l->F = temp;
    } else {
        act = ubicarventa(p, l); a = act->ant; s = act->sig;
        free(act); a->sig = s; s->ant = a;
    }
    l->NE--;
    return OKE;
}

booleano emptyventa(listaventa l) {
    if (l != NULL && l->I == NULL && l->F == NULL && l->NE == 0) return TRU;
    return FALSO;
}

mensaje setventa(int p, infoventa E, listaventa l) {
    enlaceventa Ite;
    if (l == NULL || emptyventa(l)) return EMPTY;
    if (!(p >= 0 && p < l->NE)) return INDEXOUTOFBOUND;
    Ite = ubicarventa(p, l);
    Ite->Inf = E;
    return OKE;
}

mensaje crearlistaventa(listaventa *l) {
    listaventa temp = (listaventa) malloc(sizeof(cabecera_venta));
    if (temp == NULL) return NO_MEMORY;
    temp->I = temp->F = NULL;
    temp->NE = 0;
    *l = temp;
    return OKE;
}

void liberarlistaventa(listaventa *l) {
    if (*l != NULL) {
        Vaciarlistaventa(*l);
        free(*l);
        *l = NULL;
    }
}

void Vaciarlistaventa(listaventa l) {
    while (!emptyventa(l)) borrarventa(0, l);
}