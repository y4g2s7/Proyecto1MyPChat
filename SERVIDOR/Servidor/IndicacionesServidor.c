#include <string.h>

char *indicaciones(char *mensaje){
  if(strcmp(mensaje,"hola")==0){
    return "bienvenido\n";
  } else if(strcmp(mensaje,"adios")==0){
    return "vuelve pronto\n";
  } else if(strcmp(mensaje,"Mensaje invalido")==0){
    return "No te entendi\n";
  }
  return NULL;
}


