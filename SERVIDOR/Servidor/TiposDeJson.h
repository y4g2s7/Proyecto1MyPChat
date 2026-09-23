#ifndef TIPOSDEJSON_H
#define TIPOSDEJSON_H
#include "ListaDeUsuarios.h"

char *crearJson(char *type, char *operation, char *result, char * extra, char * username, char *status, char *text, char *roomname);

char *stringListaUsuario();

void agregarSaltoLinea(char **mensaje);
#endif

