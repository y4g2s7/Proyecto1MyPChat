#include <string.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include "TiposDeJson.h"
#include <uthash.h>
#include "ListaDeUsuarios.h"
#include "IndicacionesServidor.h"

char *traduccionJSON(char *mensaje, int socket_fd){
  /* Vemos si lo que nos enviaron es JSON */
  cJSON *raiz = cJSON_Parse(mensaje);
  if(raiz == NULL){
    return NULL;
  }
  /* Vemos que tipo es */
  cJSON *tipoNodo=cJSON_GetObjectItem(raiz, "type");
  if(tipoNodo==NULL){
    return NULL;
  }
  char *tipoTexto = tipoNodo->valuestring;
  char *respuesta;
  /* Si es IDENTIFYY */
  if(strcmp(tipoTexto,"IDENTIFY")==0){
    cJSON *usernameNodo = cJSON_GetObjectItem(raiz, "username");
    if(usernameNodo==NULL){
      return NULL;
    }
    /* Mandamos a llamar a Identificar que verifica la situacion del nombre de usuario */
    respuesta = Identificar(usernameNodo,socket_fd);
  }
  cJSON_Delete(raiz);
  return respuesta;
}

char *Identificar(cJSON *usernameNodo,int socket_fd){
  char user[9];
  strncpy(user,usernameNodo->valuestring,9);
  user[8]='\0';
  MIdentify *identify = newIdentify(user);
  /* Revizamos si el nombre del usuario esta ya en la lista */
  Usuario *encontrado = NULL;
  HASH_FIND_STR(tablaUsuarios, identify->username, encontrado);
  
  char *respuesta;
  if(encontrado==NULL){
    /* Creamos un nuevo usuario */
    Usuario *nuevo=newUsuario(identify->username,socket_fd);
    /* Agregamos a la lista de ususarios */
    HASH_ADD_STR(tablaUsuarios, username, nuevo);      
    /* Agregar a la lista */
    respuesta=agregarUsuario(identify);
  } else{
    respuesta=usuarioExistente(identify);
  }
  free(identify);
  return respuesta;
  
}
