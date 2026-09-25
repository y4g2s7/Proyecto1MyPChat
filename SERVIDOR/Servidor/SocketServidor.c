#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "IndicacionesServidor.h"
#include "SocketServidor.h"
#include <errno.h>
#include "TiposDeJson.h"

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
    
    int *fdParaHilo = malloc(sizeof(int));
    *fdParaHilo = clienteFd;
    /* Creamos para cada cliente un hilo distinto */
    pthread_t hilo;
    pthread_create(&hilo, NULL, atenderCliente, fdParaHilo);

    /* Libera recursos cuando el hilo termine */
    pthread_detach(hilo);
  }
  close(servidorFd);
}

/* Funcion que pide ptharead_create con la cual trabajan los hilos */
void *atenderCliente(void *arg){
  int clienteFd = *(int *)arg;
  free(arg);
  /* Creamos un arreglo para guardar lo que nos dice en cliente */
  char buffer[1024*1024]={0};

  /* Variable para guardar cuantos bytes llevamos leidos, su funcion principal es cuando
    en una primera entrega de bytes por parte del servidor no recibimos "\n" podamos guardar
    la parte del mensaje que aun no procesamos, para que cuando nos envien el "\n" no
    sobreescribamos en char buffer respetando el espacio de bytesAcumulados */
  int bytesAcumulados=0;

  /* Variable que guarda donde inicia cada mensaje */
  char *inicioMensaje=buffer;

  /* Numero de bytes que leimos con read() si un numero positivo, si es 0 nos
     dice que el usuario se desconecto, si es negativo hubo un error*/
  ssize_t bytesLeidos;

  /* Variable para guardar si tengo que desconectar al cliente por algun error */
    bool desconexion = false;

    bool identificacion = false;
    
  /* Ciclo que termina cuando el ususario se desconecta */
  while ((bytesLeidos = read(clienteFd, buffer+bytesAcumulados, sizeof(buffer)-bytesAcumulados))>0){
    char *mensajes[50];
    
    /* Donde terminamos de recibir datos del cliente ponemos '\0' para marcar el final */
    buffer[bytesLeidos+bytesAcumulados]='\0';

    /* Actualizamos la variable bytesAcumulados sumandole los bytesLeidos
       si no hay bytes sobrantes despues se actualiza a 0*/
    bytesAcumulados+=bytesLeidos;
    
    /* Llamamos a la funcion que se encarga de separar los mensajes con \n y tratarlos */
    int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,clienteFd, &identificacion);
    
    /* Iteramos el arreglo donde guardamos las respuestas del servidor para mandarselas al cliente */
    for(int i=0;i<numeroMensajes;i++){
      
      if(escribirCompleto(clienteFd,mensajes[i],strlen(mensajes[i])) <0){
	desconexion = true;
      }

      /* LIBERAR MEMORIA */
      free(mensajes[i]);
      
      if(desconexion){
	for(int j=i+1;j<numeroMensajes;j++){
	  free(mensajes[j]);
	}
	goto fin;
      }
      
    }  
  }
 fin:
  printf("Cliente se desconectó.\n");
  close(clienteFd);

  char *usuarioPtr = getUsername(clienteFd);
  /* eliminamos de la lista de usuarios y de los cuartos */
  if(usuarioPtr != NULL){
    char usuario[9];
    strncpy(usuario, usuarioPtr, sizeof(usuario));
    usuario[8] = '\0';

    pthread_mutex_lock(&mutexUsuarios);
    Usuario *encontrado = NULL;
    HASH_FIND_STR(tablaUsuarios, usuario, encontrado);
    if(encontrado != NULL){
      HASH_DEL(tablaUsuarios, encontrado);
      free(encontrado->estado);
      free(encontrado);
    }
    pthread_mutex_unlock(&mutexUsuarios);

    pthread_mutex_lock(&mutexSalas);
    Sala *act, *tm;
    HASH_ITER(hh, tablaSalas, act, tm) {
      pthread_mutex_lock(&act->mutexUsuariosSala);
      MiembroSala *encontrada = NULL;
      HASH_FIND_STR(act->tablaUsuariosSala, usuario, encontrada);
      if(encontrada != NULL){
	HASH_DEL(act->tablaUsuariosSala, encontrada);
	free(encontrada);
      }
      pthread_mutex_unlock(&act->mutexUsuariosSala);
    }
    pthread_mutex_unlock(&mutexSalas);
  }
  return NULL;
}

/* Funcion que se asegura de que todo el mensaje se envie */
ssize_t escribirCompleto(int fd, const char *buf, size_t len){
  size_t enviados = 0;
  while (enviados < len) {
    ssize_t n = write(fd, buf + enviados, len - enviados);
    if (n < 0) {
      /* Vemos si el error es por conexion  */
      if (errno == EINTR) {
	continue;   
      }
      /* Hubo un error al enviar los datos */
      return -1;                      
    }
    /* Actializamos la variable para ver cuantos enviamos */
    enviados += (size_t)n;
  }
  return (ssize_t)enviados;
}

int procesarBuffer(char *buffer, int *bytesAcumulados, char **inicioMensaje, char *mensajes[50],bool *desconexion,int socket_fd, bool *identificacion){
  /* Variable que guarda los datos hasta que encuentre '\n' */
  char *mensajeFiltrado;

  /* Variable para iterar mensajes fuera del ciclo */
  int contador = 0;
  
  /* Ciclo que termina hasta que ya no haya '\n' en el mensaje */
  while((mensajeFiltrado=strchr(*inicioMensaje, '\n')) != NULL){

    /* Donde encontramos '\n' ponemos '\0' para marcar el fin de ese mensaje */
    *mensajeFiltrado='\0';

    /* Si el mensaje era vacio, es decir solo era un '\n' lo ignoramos */
   if(**inicioMensaje=='\0'){

      /* Actualizamos incioMensaje */
      *inicioMensaje=mensajeFiltrado+1;
      continue;
    }
   
    /* Madamos llamar indicaciones() para saber que vamos a responder */
   char *respuesta=traduccionJSON(*inicioMensaje, socket_fd, identificacion);
   
    /* Actualizamos incioMensaje */
    *inicioMensaje=mensajeFiltrado+1;

    char buscar[] = "USER_ALREADY_EXISTS";

    if((*identificacion == false) && (strstr(respuesta, buscar) == NULL)){
      respuesta = crearJson("RESPONSE","INVALID","NOT_IDENTIFIED",NULL,NULL,NULL,NULL,NULL);
      *desconexion = true;
      if(respuesta==NULL) break;
    }
    /* Si el cliente le manda algo al sevidor que no puede decifrar
       le decimos al usuario y volvemos a esperar respuesta*/
    if(respuesta == NULL){
      respuesta = crearJson("RESPONSE","INVALID","INVALID",NULL,NULL,NULL,NULL,NULL);
      *desconexion = true;
      if(respuesta==NULL) break;
    }

    /* Si es un mensaje  que no debemos regresar nada al usuario ignoramos */
    if(strcmp(respuesta,"ignora")!=0){
    /* Agregamos las respuestas a mensajes */
      mensajes[contador]=respuesta;
      contador++;
    }
  }
  /* Limpiamos el buffer de los datos que ya procesamos, recorriendo la
     informacion que aun nos falta por procesar (si es que tenemos) al inicio
     y moviendo las variables que nos ayudan a ubicarnos
     memmove(destino,origen,tamaño)*/
  memmove(buffer,*inicioMensaje,(buffer+*bytesAcumulados)-*inicioMensaje);
  *bytesAcumulados=(buffer+*bytesAcumulados)-*inicioMensaje;
  *inicioMensaje=buffer;
  return contador;
}
