#include <string.h>

char *indicaciones(char *mensaje){
  if(strcmp(mensaje,"hola")==0){
    return "bienvenido";
  } else if(strcmp(mensaje,"adios")==0){
    return "vuelve pronto";
  }
  return NULL;
}


