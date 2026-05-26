#ifndef ADMIN_BACKEND_H
#define ADMIN_BACKEND_H

#include "admin.h"

int  conectarServidorAdmin();
void desconectarServidorAdmin();
void cargarCatalogoAdmin(listaproducto cat);
int  agregarProductoAdmin(prod p);
void modificarExistenciasAdmin(const char *nombreProducto, int nuevaCantidad);
void eliminarProductoAdmin(const char *nombreProducto);

#endif