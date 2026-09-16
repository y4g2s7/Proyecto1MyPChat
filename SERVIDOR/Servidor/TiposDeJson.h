#ifndef TIPOSDEJSON_H
#define TIPOSDEJSON_H
#include "ListaDeUsuarios.h"
/* Estructura para trabajar con la creacion de un nuevo nombre de usuario */ 
typedef struct {
  char username[9];
} MIdentify;

MIdentify *newIdentify(char username[9]);
char *agregarUsuario(MIdentify *midentify);
char *usuarioExistente(MIdentify *midentify);
char *avisoNuevoUsuario(MIdentify *midentify);
char *stringListaUsuario();
char *avisoCambioEstado(Usuario *usuario);
void agregarSaltoLinea(char **mensaje);
#endif

