#ifndef TIPOSDEJSON_H
#define TIPOSDEJSON_H

typedef struct {
  char username[9];
} MIdentify;

MIdentify *newIdentify(char username[9]);
char *agregarUsuario(MIdentify *midentify);
#endif

