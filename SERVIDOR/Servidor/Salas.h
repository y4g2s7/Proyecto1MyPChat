#ifndef SALAS_H
#define SALAS_H
#include <uthash.h>
#include <pthread.h>
#include "ListaDeUsuarios.h"
 

typedef struct {
  /* llave */
  char username[9];   
  /* referencia hacia los miembros de la lista global */
  Usuario *usuario;
  UT_hash_handle hh; 
} MiembroSala;


typedef struct {
    char username[9];
    UT_hash_handle hh;
} Invitacion;


typedef struct{
  char nombre[17];
  MiembroSala *tablaUsuariosSala;
  pthread_mutex_t mutexUsuariosSala;
  Invitacion *tablaInvitados;
  UT_hash_handle hh;
}Sala;



Sala *newSala(char nombre[17]);
MiembroSala *newMiembroSala(char username[9], Usuario *usuario);
extern Sala *tablaSalas;
extern pthread_mutex_t mutexSalas;
#endif
