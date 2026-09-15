#include <stdlib.h>
#include <string.h>
#include "TiposDeJson.h"
#include <cjson/cJSON.h>
#include "ListaDeUsuarios.h"
 
MIdentify *newIdentify(char username[9]){
  MIdentify *identify = malloc(sizeof(MIdentify));
  
  strncpy(identify->username,username,sizeof(identify->username));
  return identify;
}

char *agregarUsuario(MIdentify *midentify){
  cJSON *respuesta = cJSON_CreateObject();
  cJSON_AddStringToObject(respuesta, "type", "RESPONSE");
  cJSON_AddStringToObject(respuesta, "operation", "IDENTIFY");
  cJSON_AddStringToObject(respuesta, "result", "SUCCESS");
  cJSON_AddStringToObject(respuesta, "extra", midentify->username);
  char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
  cJSON_Delete(respuesta);
  agregarSaltoLinea(&StringRespuesta);
  return StringRespuesta;
}

char *usuarioExistente(MIdentify *midentify){
  /* Crear un nuevo usuario  */
  cJSON *respuesta = cJSON_CreateObject();
  cJSON_AddStringToObject(respuesta, "type", "RESPONSE");
  cJSON_AddStringToObject(respuesta, "operation", "IDENTIFY");
  cJSON_AddStringToObject(respuesta, "result", "USER_ALREADY_EXISTS");
  cJSON_AddStringToObject(respuesta, "extra", midentify->username);
  char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
  cJSON_Delete(respuesta);
  agregarSaltoLinea(&StringRespuesta);
  return StringRespuesta;
}

char *avisoNuevoUsuario(MIdentify *midentify){
    /* Crear un nuevo usuario  */
  cJSON *respuesta = cJSON_CreateObject();
  cJSON_AddStringToObject(respuesta, "type", "NEW_USER");
  cJSON_AddStringToObject(respuesta, "username", midentify->username);
  char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
  cJSON_Delete(respuesta);
  agregarSaltoLinea(&StringRespuesta);
  return StringRespuesta;
}

/* Funcion que agrega el salto de linea para enviar al cliente y el \0 para marcar el limite en C */
void agregarSaltoLinea(char **mensaje){
  size_t len=strlen(*mensaje);
  char *tmp=realloc(*mensaje,len+2);
  if (tmp != NULL){
    *mensaje=tmp;
    (*mensaje)[len]='\n';
    (*mensaje)[len+1]='\0';
   }
}
