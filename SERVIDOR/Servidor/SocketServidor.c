#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "IndicacionesServidor.h"
#include "SocketServidor.h"

/* Funcion que se encarga de la creacion del servidor y de su capacidad
   para trabajar con multiples usuarios */
void runServidor() {
  /* Creamos el socket:
     AF_INET para tener direccion IPv4
     SOCK_STREAM para conexion tipo TCP
     0 protocolo por default */  
  int servidorFd = socket(AF_INET,SOCK_STREAM,0);

  /* Le decimos al sistema que podemos volver a usar la IP y el puerto de
     inmediato despues de cerrar un servidor */
  int booleano=1;
  setsockopt(servidorFd, SOL_SOCKET, SO_REUSEADDR,&booleano, sizeof(booleano));

  /* Variable para la direccion */
  struct sockaddr_in direccion = {0};

  /* Le decimos que tiene que ser IPv4 */
  direccion.sin_family = AF_INET;

  /* Le decimos que acepte conexion desde cualquier IP */
  direccion.sin_addr.s_addr = INADDR_ANY;

  /* Le damos un puerto */
  direccion.sin_port = htons(9000);

  bind(servidorFd, (struct sockaddr *)&direccion, sizeof(direccion));

  /* Abrimos el servidor para que los clientes se puedan conectar */
  listen(servidorFd, 1);

  printf("Escuchando en puerto 9000...\n");

  /* Ciclo infinito para la conexion de usuarios */
  while(1){
    /* Creamos un cliente diferente cada vez que alguien se conecte */
    int clienteFd=accept(servidorFd, NULL, NULL);
    printf("Cliente conectado.\n");  

    /* Creamos para cada cliente un hilo distinto */
    pthread_t hilo;
    pthread_create(&hilo, NULL, atenderCliente, &clienteFd);

    /* Libera recursos cuando el hilo termine */
    pthread_detach(hilo);
  }
  close(servidorFd);
}

/* Funcion que pide ptharead_create con la cual trabajan los hilos */
void *atenderCliente(void *arg){
  int clienteFd = *(int *)arg;
  char buffer[256]={0};
  ssize_t bytesLeidos;
  while ((bytesLeidos = read(clienteFd, buffer, sizeof(buffer)))>0){
    buffer[bytesLeidos]='\0';      
    char *respuesta=indicaciones(buffer);

    /* Si el cliente le manda algo al sevidor que no puede decifrar
       le decimos al usuario y volvemos a esperar respuesta*/
    if(respuesta == NULL){
      printf("Error no puedo captar el mensaje");
      continue;
    }
    write(clienteFd, respuesta, strlen(respuesta));
  }
  printf("Cliente se desconectó.\n");
  close(clienteFd);
  return NULL;
}
