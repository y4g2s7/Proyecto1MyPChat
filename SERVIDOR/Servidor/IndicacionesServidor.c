#include <string.h>
#include "cJSON.h"

char *indicaciones(char *mensaje){
  if(strcmp(mensaje,"hola")==0){
    return "bienvenido\n";
  } else if(strcmp(mensaje,"adios")==0){
    return "vuelve pronto\n";
  } else if(strcmp(mensaje,"Mensaje invalido")==0){
    return "No te entendi\n";
  }
  return NULL;
}

char *JSON(char *mensaje){
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
    char *username = usernameNodo->valuestring;
    
    cJSON *respuesta = cJSON_CreateObject();
    cJSON_AddStringToObject(respuesta, "type", "RESPONSE");
    cJSON_AddStringToObject(respuesta, "operation", "IDENTIFY");
    cJSON_AddStringToObject(respuesta, "result", "SUCCESS");
    cJSON_AddStringToObject(respuesta, "extra", username);
    char *StringRespuesta= cJSON_PrintUnformatted(respuesta);
    
    cJSON_Delete(raiz);
    return StringRespuesta;
  }
  return NULL;
}
