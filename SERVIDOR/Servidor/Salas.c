#include <uthash.h>
#include "Salas.h"
#include <pthread.h>
 
Sala *tablaSalas = NULL;

pthread_mutex_t mutexSalas = PTHREAD_MUTEX_INITIALIZER;

Sala *newSala(char nombre[17]){
  Sala *nueva = malloc(sizeof(Sala)); 
  if(nueva == NULL){                                                        
    return NULL;                                                            
  }
  strncpy(nueva->nombre,nombre,sizeof(nueva->nombre));
  nueva->nombre[16]='\0';
  Usuario *tablaUsuariosSala = NULL;
  
  nueva->tablaUsuariosSala=tablaUsuariosSala;
  
  pthread_mutex_init(&nueva->mutexUsuariosSala, NULL);

 return nueva;
} 
