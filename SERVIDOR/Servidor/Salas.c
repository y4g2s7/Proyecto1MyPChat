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
  MiembroSala *tablaUsuariosSala = NULL;

  nueva->tablaUsuariosSala=tablaUsuariosSala;
  
  Invitacion *tablaInvitados = NULL;
    
  nueva->tablaInvitados=tablaInvitados;
  pthread_mutex_init(&nueva->mutexUsuariosSala, NULL);

 return nueva;
}

MiembroSala *newMiembroSala(char username[9], Usuario *usuario){
  MiembroSala *nuevo = malloc(sizeof(MiembroSala)); 
  if(nuevo == NULL){                                                        
    return NULL;                                                            
  }
  strncpy(nuevo->username,username,sizeof(nuevo->username));
  nuevo->username[8]='\0';
  nuevo->usuario = usuario;
  return nuevo;
}
