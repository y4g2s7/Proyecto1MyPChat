#ifndef SALAS_H
#define SALAS_H
#include <uthash.h>
#include <pthread.h>
#include "ListaDeUsuarios.h"
 
typedef struct{
  char nombre[17];
  Usuario *tablaUsuariosSala;
  pthread_mutex_t mutexUsuariosSala;
  UT_hash_handle hh;
}Sala;

Sala *newSala(char nombre[17]);

extern Sala *tablaSalas;
extern pthread_mutex_t mutexSalas;
#endif
