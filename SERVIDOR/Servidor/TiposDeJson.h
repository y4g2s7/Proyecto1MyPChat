#ifndef TIPOSDEJSON_H
#define TIPOSDEJSON_H
#include "ListaDeUsuarios.h"
#include "Salas.h"

char *crearJson(char *type, char *operation, char *result, char * extra, char * username, char *status, char *text, char *roomname);

char *stringListaUsuario();

char *stringListaUsuarioSala(Sala *salaEncontrada);

void agregarSaltoLinea(char **mensaje);
#endif

