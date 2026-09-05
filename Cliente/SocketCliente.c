#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Hilos.h"

void runCliente(){
  /* Creamos el socket:
     AF_INET para tener direccion IPv4
     SOCK_STREAM para conexion tipo TCP
     0 protocolo por default */  
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  /* Variable para la direccion */
  struct sockaddr_in direccion = {0};

  /* Le decimos que tiene que ser IPv4 */
  direccion.sin_family = AF_INET;

  /* Le damos un puerto */
  direccion.sin_port = htons(9000);
  
  /* Le damos la ip del serivor */
  inet_pton(AF_INET, "127.0.0.1", &direccion.sin_addr);

  /* Conectamos al socket con el serviodor */
  int coneccion = connect(socket_fd, (struct sockaddr *)&direccion, sizeof(direccion));
  /* Si se pudo establecer conexion continuamos */
  if(coneccion == 0){
    printf("Te haz conectado al servidor que deseas hacer?\n");
    
    /* Creacion del hilo para escuchar al servidor */
    pthread_t hiloEscucha;
    pthread_create(&hiloEscucha,NULL,fEscucha,&socket_fd);

    /* Creacion del hilo para escribir hacia el servidor */
    pthread_t hiloEscribe;
    pthread_create(&hiloEscribe,NULL,fEscribe,&socket_fd);

    /* Hacemos que el socket espero a que los hilos terminen para poder cerrarse */ 
    pthread_join(hiloEscucha, NULL);
    pthread_join(hiloEscribe, NULL);
  } else {
    printf("Error, no se pudo entablar conexion\n");
  }
  close(socket_fd);
}
