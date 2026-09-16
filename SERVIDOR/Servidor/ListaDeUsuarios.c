#include <uthash.h>
#include "ListaDeUsuarios.h"
#include <pthread.h>

/* Variable que vive entre archivos para modificar y leer la lista de usuarios */
/* La declaramos NULL */
Usuario *tablaUsuarios = NULL;

pthread_mutex_t mutexUsuarios = PTHREAD_MUTEX_INITIALIZER;

Usuario *newUsuario(char username[9],int socket_fd){
  Usuario *nuevo = malloc(sizeof(Usuario)); 
  if(nuevo == NULL){                                                        
    return NULL;                                                            
  }
  strncpy(nuevo->username,username,sizeof(nuevo->username));
  nuevo->username[8]='\0';
  nuevo->socket_fd = socket_fd;
  nuevo->estado = strdup("ACTIVE");
  return nuevo;
}
 
