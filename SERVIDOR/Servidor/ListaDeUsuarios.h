#ifndef LISTADEUSUARIOS_H  
#define LISTADEUSUARIOS_H
#include <uthash.h>
#include <pthread.h>


/* Nodos del diccionario */
typedef struct {
  char *estado;
  /* la llave */
  char username[9];      
  /* el valor asociado */
  int socket_fd;            
  /* el campo especial que uthash necesita */
  UT_hash_handle hh;       
} Usuario;
 
extern Usuario *tablaUsuarios;
extern pthread_mutex_t mutexUsuarios;
Usuario *newUsuario(char username[9],int socket_fd);

#endif
