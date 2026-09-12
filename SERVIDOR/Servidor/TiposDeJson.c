#include <stdlib.h>
#include <string.h>
#include "TiposDeJson.h"
#include <cjson/cJSON.h>
 
MIdentify *newIdentify(char username[9]){
  MIdentify *identify = malloc(sizeof(MIdentify));
  
  strncpy(identify->username,username,sizeof(identify->username));
  return identify;
}

char *agregarUsuario(MIdentify *midentify){
  /* verificar que el nombre no esta en la lista de usuarios */
  /* Crear un nuevo usuario  */
  cJSON *respuesta = cJSON_CreateObject();
  cJSON_AddStringToObject(respuesta, "type", "RESPONSE");
  cJSON_AddStringToObject(respuesta, "operation", "IDENTIFY");
  cJSON_AddStringToObject(respuesta, "result", "SUCCESS");
  cJSON_AddStringToObject(respuesta, "extra", midentify->username);
  char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
  cJSON_Delete(respuesta);

  size_t len=strlen(StringRespuesta);
  char *tmp=realloc(StringRespuesta,len+1);
  if (tmp != NULL){
    StringRespuesta=tmp;
    StringRespuesta[len]='\n';
  }
  /* Hacer otro json para avisrles a los demas usuarios
     mediante la lista que se conecto uno nuevo */  
  return StringRespuesta;
}
