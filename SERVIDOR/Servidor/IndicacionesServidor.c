#include <string.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include "TiposDeJson.h"
#include <uthash.h>
#include "IndicacionesServidor.h"
#include <pthread.h>
#include <unistd.h>
#include "SocketServidor.h"
#include "Salas.h"

char *traduccionJSON(char *mensaje, int socket_fd, bool *identificacion){
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
    respuesta = Identificar(usernameNodo,socket_fd,identificacion);
  } else if(!(*identificacion)){
     /* cualquier otro tipo de mensaje requiere estar identificado primero */
    respuesta = crearJson("RESPONSE","INVALID","NOT_IDENTIFIED",NULL,NULL,NULL,NULL,NULL);
  } else if(strcmp(tipoTexto,"USERS")==0){
    respuesta=stringListaUsuario();
  } else if(strcmp(tipoTexto, "STATUS")==0){
    cJSON *statusNodo = cJSON_GetObjectItem(raiz, "status");
    if(statusNodo==NULL || !cJSON_IsString(statusNodo)){
      goto cleanup;
    }
    respuesta = cambioEstado(statusNodo,socket_fd);
  } else if(strcmp(tipoTexto,"TEXT")==0){
    cJSON *destinatarioNodo = cJSON_GetObjectItem(raiz, "username");
    if(destinatarioNodo==NULL || !cJSON_IsString(destinatarioNodo)){
      goto cleanup;
    }
    cJSON *mensajeNodo = cJSON_GetObjectItem(raiz, "text");
    if(mensajeNodo==NULL || !cJSON_IsString(mensajeNodo)){
      goto cleanup;
    }
    respuesta = mensajePrivado(destinatarioNodo,mensajeNodo,socket_fd);
  } else if(strcmp(tipoTexto, "PUBLIC_TEXT")==0){
    
    cJSON *mensajePublicoNodo = cJSON_GetObjectItem(raiz, "text");
    if(mensajePublicoNodo==NULL || !cJSON_IsString(mensajePublicoNodo)){
      goto cleanup;
    }
    respuesta = mensajePublico(mensajePublicoNodo,socket_fd);
  } else if (strcmp(tipoTexto, "NEW_ROOM")==0){
    cJSON *nombreSalaNodo = cJSON_GetObjectItem(raiz, "roomname");
    if(nombreSalaNodo==NULL || !cJSON_IsString(nombreSalaNodo)){
      goto cleanup;
    }
    respuesta = sala(nombreSalaNodo,socket_fd);
  }  else if (strcmp(tipoTexto, "INVITE")==0){
    cJSON *nombreSalaNodo = cJSON_GetObjectItem(raiz, "roomname");
    if(nombreSalaNodo==NULL || !cJSON_IsString(nombreSalaNodo)){
      goto cleanup;
    }
    cJSON *usuariosNodo = cJSON_GetObjectItem(raiz, "usernames");
    if(usuariosNodo==NULL || !cJSON_IsArray(usuariosNodo)){
      goto cleanup;
    }
    respuesta =invitarSala(nombreSalaNodo,usuariosNodo,socket_fd);
  }
 cleanup:
  cJSON_Delete(raiz);
  return respuesta;
}

 char *invitarSala(cJSON *nombreSalaNodo, cJSON *usuariosNodo,int socket_fd){
   char nombre[17];
   strncpy(nombre,nombreSalaNodo->valuestring,17);
   
  if(nombre[0]=='\0'){
    return NULL;
  }
  nombre[16]='\0';
  
  char *invitador=getUsername(socket_fd);
 pthread_mutex_lock(&mutexSalas);
  Sala *salaEncontrada = NULL;
  HASH_FIND_STR(tablaSalas, nombre, salaEncontrada);
  char *respuesta;
  if(salaEncontrada ==NULL){
    respuesta = crearJson("RESPONSE", "INVITE", "NO_SUCH_ROOM",nombre,NULL,NULL,NULL,NULL);
    
  } else{
    pthread_mutex_lock(&salaEncontrada->mutexUsuariosSala);
    MiembroSala *miembroEncontrado = NULL;
    HASH_FIND_STR(salaEncontrada->tablaUsuariosSala,invitador,miembroEncontrado);

    if(miembroEncontrado==NULL){
      pthread_mutex_unlock(&salaEncontrada->mutexUsuariosSala);
      return NULL;
    }
    
    pthread_mutex_unlock(&salaEncontrada->mutexUsuariosSala);
    
    pthread_mutex_lock(&mutexUsuarios);
    cJSON *elemento = NULL;
    cJSON_ArrayForEach(elemento, usuariosNodo) {
      
      Usuario *UsuarioEncontrado = NULL;
      HASH_FIND_STR(tablaUsuarios,elemento->valuestring,UsuarioEncontrado);
      if(UsuarioEncontrado==NULL){
	respuesta = crearJson("RESPONSE", "INVITE", "NO_SUCH_USER",elemento->valuestring,NULL,NULL,NULL,NULL);
	pthread_mutex_unlock(&mutexUsuarios);
	pthread_mutex_unlock(&mutexSalas);
	return respuesta;
      }
    }
    
    
    char *mensaje = crearJson("INVITATION",NULL,NULL,NULL,invitador,NULL,NULL,nombre);
    
    cJSON *elemento2 = NULL;
    cJSON_ArrayForEach(elemento2, usuariosNodo) {
      Usuario *encontrado2 = NULL;
      HASH_FIND_STR(tablaUsuarios,elemento2->valuestring,encontrado2);

      pthread_mutex_lock(&salaEncontrada->mutexUsuariosSala);
      Invitacion *invEncontrada = NULL;
      HASH_FIND_STR(salaEncontrada->tablaInvitados,elemento2->valuestring,invEncontrada);
      if(invEncontrada == NULL){
	Invitacion *inv = malloc(sizeof(Invitacion));
	strncpy(inv->username, elemento2->valuestring, sizeof(inv->username));
	inv->username[8] = '\0';

	
	HASH_ADD_STR(salaEncontrada->tablaInvitados, username, inv);
	pthread_mutex_unlock(&salaEncontrada->mutexUsuariosSala);
	
	
	escribirCompleto(encontrado2->socket_fd,mensaje,strlen(mensaje));
      }
      pthread_mutex_unlock(&salaEncontrada->mutexUsuariosSala);
    }
    pthread_mutex_unlock(&mutexUsuarios);
    free(mensaje);
    respuesta = "ignora";
    
  }
  pthread_mutex_unlock(&mutexSalas);
  return respuesta;
 }

char *sala(cJSON *nombreSalaNodo, int socket_fd){
  char nombre[17];
  strncpy(nombre,nombreSalaNodo->valuestring,17);
  
  if(nombre[0]=='\0'){
    return NULL;
  }
  nombre[16]='\0';
  
  pthread_mutex_lock(&mutexSalas);
  Sala *encontrado = NULL;
  HASH_FIND_STR(tablaSalas, nombre, encontrado);
  char *respuesta;
  if(encontrado ==NULL){
    Sala *s = newSala(nombre);
    if(s == NULL){
      pthread_mutex_unlock(&mutexSalas);
      return NULL;
    }
    HASH_ADD_STR(tablaSalas, nombre, s);
    respuesta = crearJson("RESPONSE", "NEW_ROOM", "SUCCESS",nombre,NULL,NULL,NULL,NULL);
    char *username = getUsername(socket_fd);
    
    pthread_mutex_lock(&mutexUsuarios);
    Usuario *encontrado = NULL;
    HASH_FIND_STR(tablaUsuarios, username, encontrado);
    
    if (encontrado != NULL) {

      MiembroSala *ms = newMiembroSala(username,encontrado);
      if(ms == NULL){
	pthread_mutex_unlock(&mutexUsuarios);
	pthread_mutex_unlock(&mutexSalas);
	free(respuesta);
	return NULL;
      }
      pthread_mutex_lock(&s->mutexUsuariosSala);
      
      HASH_ADD_STR(s->tablaUsuariosSala, username, ms);
      
      pthread_mutex_unlock(&s->mutexUsuariosSala);
    }
    pthread_mutex_unlock(&mutexUsuarios);
    
  } else{
    respuesta = crearJson("RESPONSE", "NEW_ROOM", "ROOM_ALREADY_EXISTS",nombre,NULL,NULL,NULL,NULL);
  }
  pthread_mutex_unlock(&mutexSalas);
  return respuesta;
}

char *mensajePublico(cJSON *mensajePublicoNodo, int socket_fd){
  char *respuesta;
  char *mensaje = mensajePublicoNodo->valuestring;
  if(mensaje[0]=='\0'){
    return NULL;
  }
  char* remitente = getUsername(socket_fd);
  char *json = crearJson("PUBLIC_TEXT",NULL,NULL,NULL,remitente,NULL,mensaje,NULL);
  pthread_mutex_lock(&mutexUsuarios);
  Usuario *act, *tm;
    HASH_ITER(hh, tablaUsuarios, act, tm) {
      if(act->socket_fd!=socket_fd){
	escribirCompleto(act->socket_fd, json, strlen(json));   
      }
    }
  pthread_mutex_unlock(&mutexUsuarios);
  free(json);
  return respuesta = "ignora"; 
}

char *mensajePrivado(cJSON *destinatarioNodo, cJSON *mensajeNodo, int socket_fd){
  char *respuesta;
  char *destinatario = destinatarioNodo->valuestring;
  char *mensaje = mensajeNodo->valuestring;
  if(mensaje[0]=='\0'){
    return NULL;
    
  }
  char *remitente = getUsername(socket_fd);
  pthread_mutex_lock(&mutexUsuarios);
  /* Buscamos el usuario al que el usuario le quiere enviar el mensaje */
  Usuario *encontrado = NULL;
  HASH_FIND_STR(tablaUsuarios, destinatario, encontrado);
  
  
  if(encontrado ==NULL){
    respuesta = crearJson("RESPONSE","TEXT","NO_SUCH_USER", destinatario, NULL, NULL,NULL,NULL);
    pthread_mutex_unlock(&mutexUsuarios);
  } else{
    
    char *json = crearJson("TEXT_FROM",NULL,NULL,NULL,remitente,NULL,mensaje,NULL);
    
    escribirCompleto(encontrado->socket_fd, json, strlen(json));
    pthread_mutex_unlock(&mutexUsuarios);
    respuesta = "ignora";
    free(json);
  }
  return respuesta;
}

char *getUsername(int socket_fd){
  char* username=NULL;
  pthread_mutex_lock(&mutexUsuarios);
  /* Iteramos el diccionario para buscar el usuario del socket */
  Usuario *actual, *tmp;
  HASH_ITER(hh, tablaUsuarios, actual, tmp){
    if(actual->socket_fd==socket_fd){
      username = actual->username;
    }	  
  }
  pthread_mutex_unlock(&mutexUsuarios);
  return username;
}

char *cambioEstado(cJSON *statusNodo, int socket_fd){
  char *estado= statusNodo->valuestring;
  /* Corroboramos si es algun estado permitido, regresamos NULL */
  if(strcmp(estado,"ACTIVE")==0 || strcmp(estado,"AWAY")==0 || strcmp(estado,"BUSY")==0){
    char *json=NULL;
    pthread_mutex_lock(&mutexUsuarios);
    /* Iteramos el diccionario para buscar el usuario al que hay que cambiar el estado */
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
	  json = crearJson("NEW_STATUS",NULL,NULL,NULL,actual->username,actual->estado,NULL,NULL);
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
	escribirCompleto(act->socket_fd, json, strlen(json));
        
      }
    }
    pthread_mutex_unlock(&mutexUsuarios);
    free(json);
    /* Regresamos ignora para no agregar la respuesta al arreglo y no mandarlo al usuario */
    return "ignora";
  } return NULL;
}
 
char *Identificar(cJSON *usernameNodo,int socket_fd, bool *identificacion){
  if(*identificacion) return NULL;
  char user[9];
  strncpy(user,usernameNodo->valuestring,9);

  if(user[0]=='\0'){
    return NULL;
  }
  user[8]='\0';
  
  pthread_mutex_lock(&mutexUsuarios);
  /* Revizamos si el nombre del usuario esta ya en la lista */
  Usuario *encontrado = NULL;
  HASH_FIND_STR(tablaUsuarios, user, encontrado);
  char *respuesta;
  if(encontrado==NULL){

    char* username=NULL;
    
    /* Iteramos el diccionario para buscar el usuario del socket */
    Usuario *a, *t;
    HASH_ITER(hh, tablaUsuarios, a, t){
      if(a->socket_fd==socket_fd){
	username = a->username;
      }	  
    }
    /* Si encontramos un usuario con ese socket regresamos NULL pues dos usuarios estan con el mismo */
    if(username!=NULL){
      pthread_mutex_unlock(&mutexUsuarios);
      return NULL;
    }
    /* Creamos un nuevo usuario */
    Usuario *nuevo=newUsuario(user,socket_fd);

    /* Verificacion de memoria */
    if(nuevo == NULL){
      pthread_mutex_unlock(&mutexUsuarios);
      /* free(identify); */
      return NULL;
    }
    /* Actualizamos que ya hubo una identificacion correcta  */
    *identificacion = true;
    /* Agregamos a la lista de usuarios */
    HASH_ADD_STR(tablaUsuarios, username, nuevo);      
    /* Agregar a la lista */
    
    respuesta=crearJson("RESPONSE","IDENTIFY","SUCCESS",user, NULL, NULL,NULL,NULL);

    /* Iteramos la lista de usuarios para avisar que un nuevo usuario se conecto */
    char *aviso = crearJson("NEW_USER",NULL,NULL,NULL,user,NULL,NULL,NULL);
    Usuario *actual, *tmp;
    HASH_ITER(hh, tablaUsuarios, actual, tmp) {
      if(actual->socket_fd==socket_fd){
	continue;
      }
      escribirCompleto(actual->socket_fd, aviso, strlen(aviso));
      
    }
    free(aviso);
  } else{
    respuesta=crearJson("RESPONSE","IDENTIFY","USER_ALREADY_EXISTS",user,NULL, NULL,NULL,NULL);
  }
  pthread_mutex_unlock(&mutexUsuarios);
  /* free(identify); */
  return respuesta;
  
}
