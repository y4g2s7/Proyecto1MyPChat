#include <uthash.h>
#include "ListaDeUsuarios.h"

/* Variable que vive entre archivos para modificar y leer la lista de usuarios */
/* extern Usuario *tablaUsuarios; */
/* La declaramos NULL */
Usuario *tablaUsuarios = NULL;

Usuario *newUsuario(char username[9],int socket_fd){
  Usuario *nuevo = malloc(sizeof(Usuario)); 
  if(nuevo == NULL){                                                        
    return NULL;                                                            
  }
  strncpy(nuevo->username,username,sizeof(nuevo->username));
  nuevo->username[8]='\0';
  nuevo->socket_fd = socket_fd;
  return nuevo;
}
 
