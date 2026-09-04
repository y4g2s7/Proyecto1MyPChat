#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Hilos.h"

int main() {
  //Creamos el socket
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in direccion = {0};
  direccion.sin_family = AF_INET;

  //Le damos un puerto
  direccion.sin_port = htons(9000);

  //Le damos la ip del serivor
  inet_pton(AF_INET, "127.0.0.1", &direccion.sin_addr);

  //Conectamos con el serviodor
  connect(socket_fd, (struct sockaddr *)&direccion, sizeof(direccion));
  printf("Te haz conectado al servidor que deseas hacer?\n");
    
  //Creacion del hilo para escuchar al servidor
  pthread_t hiloEscucha;
  pthread_create(&hiloEscucha,NULL,fEscucha,&socket_fd);

  //Creacion del hilo para escribir hacia el servidor
  pthread_t hiloEscribe;
  pthread_create(&hiloEscribe,NULL,fEscribe,&socket_fd);

  pthread_join(hiloEscucha, NULL);
  pthread_join(hiloEscribe, NULL);

  close(socket_fd);
  return 0;
} 
