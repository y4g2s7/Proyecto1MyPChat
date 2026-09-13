#ifndef LISTADEUSUSARIOS_H  
#define LISTADEUSUARIOS_H
#include <uthash.h>


/* Nodos del diccionario */
typedef struct {
  /* la llave */
  char username[9];       
  /* el valor asociado */
  int socket_fd;            
  /* el campo especial que uthash necesita */
  UT_hash_handle hh;       
} Usuario;

extern Usuario *tablaUsuarios;
Usuario *newUsuario(char username[9],int socket_fd);

#endif
