#include <stdlib.h>
#include <string.h>
#include "TiposDeJson.h"
#include <cjson/cJSON.h>

char *crearJson(char *type, char *operation, char *result, char * extra, char * username, char *status, char *text, char *roomname){
  cJSON *respuesta = cJSON_CreateObject();
  
  if(type) cJSON_AddStringToObject(respuesta, "type", type);
  if(operation) cJSON_AddStringToObject(respuesta, "operation", operation);
  if(result) cJSON_AddStringToObject(respuesta, "result", result);
  if(extra) cJSON_AddStringToObject(respuesta, "extra", extra);
  if(username) cJSON_AddStringToObject(respuesta, "username", username);
  if(status) cJSON_AddStringToObject(respuesta, "status", status);
  if(text) cJSON_AddStringToObject(respuesta, "text", text);
  if(roomname) cJSON_AddStringToObject(respuesta, "roomname", roomname);
  
  char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
  cJSON_Delete(respuesta);
  agregarSaltoLinea(&StringRespuesta);
  return StringRespuesta;
}

char *stringListaUsuario(){
  cJSON *respuesta = cJSON_CreateObject();
  cJSON *usuarios = cJSON_CreateObject();

  cJSON_AddStringToObject(respuesta, "type", "USER_LIST");
  pthread_mutex_lock(&mutexUsuarios);
  Usuario *actual, *tmp;
  HASH_ITER(hh, tablaUsuarios, actual, tmp) {
    cJSON_AddStringToObject(usuarios, actual->username, actual->estado);
  } 
  pthread_mutex_unlock(&mutexUsuarios);
  cJSON_AddItemToObject(respuesta, "users", usuarios);
  char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
  cJSON_Delete(respuesta);
  agregarSaltoLinea(&StringRespuesta);
  return StringRespuesta;
}


/* Funcion que agrega el salto de linea para enviar al cliente y el \0 para marcar el limite en C */
void agregarSaltoLinea(char **mensaje){
  if(mensaje == NULL || *mensaje == NULL) return;
  size_t len=strlen(*mensaje);
  char *tmp=realloc(*mensaje,len+2);
  if (tmp != NULL){
    *mensaje=tmp;
    (*mensaje)[len]='\n';
    (*mensaje)[len+1]='\0';
   }
}
