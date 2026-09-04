#include <string.h>

char *indicacionesC(char *mensaje){
  if(strcmp(mensaje,"saluda\n")==0){
    return "hola";
  } else if(strcmp(mensaje,"despidete\n")==0){
    return "adios";
  }else if(strcmp(mensaje,"salir\n")==0){
    return "exit";
  }
  return NULL;
}
