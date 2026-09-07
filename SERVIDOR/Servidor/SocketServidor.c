#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "IndicacionesServidor.h"
void runServidor() {
    int servidorFd = socket(AF_INET,SOCK_STREAM,0);

    int booleano=1;
    setsockopt(servidorFd, SOL_SOCKET, SO_REUSEADDR,&booleano, sizeof(booleano));

    struct sockaddr_in direccion = {0};
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(9000);

    bind(servidorFd, (struct sockaddr *)&direccion, sizeof(direccion));
    listen(servidorFd, 1);

    printf("Escuchando en puerto 9000...\n");
    int clienteFd=accept(servidorFd, NULL, NULL);
    printf("Cliente conectado.\n");
    
    char buffer[256]={0};
    ssize_t bytesLeidos;
    while ((bytesLeidos = read(clienteFd, buffer, sizeof(buffer)))>0){
      buffer[bytesLeidos]='\0';      
      char *respuesta=indicaciones(buffer);
      if(respuesta == NULL){
	printf("Error no puedo captar el mensaje");
	continue;
      }
      write(clienteFd, respuesta, strlen(respuesta));
    }
    printf("Cliente se desconectó.\n");
    close(clienteFd);
    close(servidorFd);
}

