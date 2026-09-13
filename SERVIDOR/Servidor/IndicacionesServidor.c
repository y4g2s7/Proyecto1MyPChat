#include <string.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include "TiposDeJson.h"

char *traduccionJSON(char *mensaje){
  cJSON *raiz = cJSON_Parse(mensaje);
  if(raiz == NULL){
    return NULL;
  }
  cJSON *tipoNodo=cJSON_GetObjectItem(raiz, "type");
  if(tipoNodo==NULL){
    return NULL;
  }
  char *tipoTexto = tipoNodo->valuestring;
  
  if(strcmp(tipoTexto,"IDENTIFY")==0){
    cJSON *usernameNodo = cJSON_GetObjectItem(raiz, "username");
    
    if(usernameNodo==NULL){
      return NULL;
    }
    
    char user[9];
    strncpy(user,usernameNodo->valuestring,9);
    user[8]='\0';
    MIdentify *identify = newIdentify(user);
    cJSON_Delete(raiz);
    char *respuesta=agregarUsuario(identify);
    free(identify);
    return respuesta;
  }
  return NULL;
}
