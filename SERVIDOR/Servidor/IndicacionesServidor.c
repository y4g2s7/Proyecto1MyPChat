#include <string.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include "TiposDeJson.h"
#include <uthash.h>
#include "IndicacionesServidor.h"
#include <pthread.h>
#include <unistd.h>

char *traduccionJSON(char *mensaje, int socket_fd){
  /* Vemos si lo que nos enviaron es JSON */
  cJSON *raiz = cJSON_Parse(mensaje);
  if(raiz == NULL){
    return NULL;
  }
  char *respuesta=NULL;

  /* Vemos que tipo es */
  cJSON *tipoNodo=cJSON_GetObjectItem(raiz, "type");
  if(tipoNodo==NULL || !cJSON_IsString(tipoNodo)){
    goto cleanup;
  }
  
  char *tipoTexto = tipoNodo->valuestring;
  
  /* Si es IDENTIFYY */
  if(strcmp(tipoTexto,"IDENTIFY")==0){
    cJSON *usernameNodo = cJSON_GetObjectItem(raiz, "username");
    if(usernameNodo==NULL || !cJSON_IsString(usernameNodo)){
      goto cleanup;
    }
    /* Mandamos a llamar a Identificar que verifica la situacion del nombre de usuario */
    respuesta = Identificar(usernameNodo,socket_fd);
  } else if(strcmp(tipoTexto,"USERS")==0){
    respuesta=stringListaUsuario();
  } else if(strcmp(tipoTexto, "STATUS")==0){
    cJSON *statusNodo = cJSON_GetObjectItem(raiz, "status");
    if(statusNodo==NULL || !cJSON_IsString(statusNodo)){
      goto cleanup;
    }
    respuesta = cambioEstado(statusNodo,socket_fd);
  }
 cleanup:
  cJSON_Delete(raiz);
  return respuesta;
}
 
char *cambioEstado(cJSON *statusNodo, int socket_fd){
  char *estado= statusNodo->valuestring;
  /* Corroboramos si es algun estado permitido, regresamos NULL */
  if(strcmp(estado,"ACTIVE")==0 || strcmp(estado,"AWAY")==0 || strcmp(estado,"BUSY")==0){
    char *json=NULL;
    pthread_mutex_lock(&mutexUsuarios);
    /* Iteramos el diccionario para buscar el usuario al que hay que cmabiar el estado */
    Usuario *actual, *tmp;
    HASH_ITER(hh, tablaUsuarios, actual, tmp) {
      if(actual->socket_fd==socket_fd){
	/* Si el nuevo estado es el mismo que ya teniamos regresamos NULL */
	if(strcmp(actual->estado,estado)==0){
	  pthread_mutex_unlock(&mutexUsuarios);
	  return NULL;
	} else{
	  /* Proteccion si no hay memoria */
	  char *copia = strdup(estado);
	  if (copia == NULL) {
	    pthread_mutex_unlock(&mutexUsuarios);
	    return NULL;
	  }
	  /* Actualizamos el estado y hacemos el json para enviarle a los demas usuarios */
	  free(actual->estado);
	  actual->estado = copia;
	  json = avisoCambioEstado(actual);
	  break;
	}
      }
    }
    /* Si el json es null quiere decir que no encontramos el usuario al que se iba a cambiar de estado */
    if(json == NULL){
      pthread_mutex_unlock(&mutexUsuarios);
      return NULL;
    }

    /* Iteramos el diccionario para enviarle a los demas usuarios el aviso */
    Usuario *act, *tm;
    HASH_ITER(hh, tablaUsuarios, act, tm) {
      if(act->socket_fd!=socket_fd){
	write(act->socket_fd, json, strlen(json));
      }
    }
    pthread_mutex_unlock(&mutexUsuarios);
    free(json);
    /* Regresamos ignora para no agregar la respuesta al arreglo y no mandarlo al usuario */
    return "ignora";
  } return NULL;
}
 
char *Identificar(cJSON *usernameNodo,int socket_fd){
  char user[9];
  strncpy(user,usernameNodo->valuestring,9);
  user[8]='\0';
  MIdentify *identify = newIdentify(user);
  
  pthread_mutex_lock(&mutexUsuarios);
  /* Revizamos si el nombre del usuario esta ya en la lista */
  Usuario *encontrado = NULL;
  HASH_FIND_STR(tablaUsuarios, identify->username, encontrado);
  
  char *respuesta;
  if(encontrado==NULL){
    /* Creamos un nuevo usuario */
    Usuario *nuevo=newUsuario(identify->username,socket_fd);

    if(nuevo == NULL){
      pthread_mutex_unlock(&mutexUsuarios);
      free(identify);
      return NULL;
    }
    /* Agregamos a la lista de usuarios */
    HASH_ADD_STR(tablaUsuarios, username, nuevo);      
    /* Agregar a la lista */
    respuesta=agregarUsuario(identify);

    /* Iteramos la lista de usuarios para avisar que un nuevo usuario se conecto */
    char *aviso = avisoNuevoUsuario(identify);
    Usuario *actual, *tmp;
    HASH_ITER(hh, tablaUsuarios, actual, tmp) {
      if(actual->socket_fd==socket_fd){
	continue;
      }
      write(actual->socket_fd, aviso, strlen(aviso));
    }
    free(aviso);
  } else{
    respuesta=usuarioExistente(identify);
  }
  pthread_mutex_unlock(&mutexUsuarios);
  free(identify);
  return respuesta;
  
}
