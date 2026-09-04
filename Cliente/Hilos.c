#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "IndicacionesCliente.h"

void *fEscucha(void *arg) {
  int socket_fd = *(int *)arg;
  
  char buffer[256] = {0};
  ssize_t bytes_leidos;
  while((bytes_leidos = read(socket_fd, buffer, sizeof(buffer)))>0){
    
    printf("El servidor respondió: %.*s\n", (int)bytes_leidos, buffer);
      
  }
  printf("fEscucha terminó\n");
  return NULL;
}

void *fEscribe(void *arg) {
    int socket_fd = *(int *)arg;
    while(1){
      //Arreglo para guardar lo que pide el usuario
      char peticion[256] = {0};
      //Lectura del usuario
      fgets(peticion,sizeof(peticion),stdin);
      //Traduccion
      char *solicitud=indicacionesC(peticion);
      if(solicitud!=NULL){
	//El usuario pidio salir, cerramos el socket y salimos
	if(strcmp(solicitud,"exit")==0){
	  printf("Cerrando socket...\n");
	  shutdown(socket_fd, SHUT_RDWR);
	  close(socket_fd);
	  return NULL;
	}
	//Escribimos lo traducido para que el servidor lo entienda
	write(socket_fd,solicitud,strlen(solicitud));
      } else{
	//Caso donde no hay una opcion para lo que escribe el usuario, manejamos aqui el null y ya no sube 
	char *respuesta = "Mensaje invalido";
	write(socket_fd,respuesta,strlen(respuesta));
      }
    }
    return NULL;
}
