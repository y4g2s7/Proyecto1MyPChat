#include <assert.h>
#include <string.h>
#include "Servidor/SocketServidor.h"
#include <stdbool.h>

/* Prueba unitaria que reviza que ignora mensajes vacios \n\n */
void TestMensajeVacio(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"IDENTIFY\",\"username\":\"Kimberly\"}\n\n{\"type\":\"IDENTIFY\",\"username\":\"Kimberly\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion);
  /* Aunque pasame 3 \n solo debe de guardar 2 mensajes  */
  assert(2==numeroMensajes);
  /* Revisemos el contenido de lo guardado coincide a lo esperado */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Kimberly\"}\n")==0);
  assert(strcmp(mensajes[1],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Kimberly\"}\n")==0);
}

/* Prueba unitaria que reviza si se se guarda un mensaje que aun no tiene \n */
/* para despues unirlo cuando nos llegue otro mensaje */
void TestMensajeSinSeparador(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[50]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  strcpy(buffer, "Este mensaje no tiene el separador");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes, &desconexion);
  /* No debemos tener ningun mensaje procesado */
  assert(0==numeroMensajes);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == strlen("Este mensaje no tiene el separador"));
  assert(strcmp(buffer,"Este mensaje no tiene el separador")==0);
}

void TestMensajeSinSeparadorCompletado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[50]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  strcpy(buffer, "Este mensaje no tiene el separador");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion);
  /* No debemos tener ningun mensaje procesado */
  assert(0==numeroMensajes);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == strlen("Este mensaje no tiene el separador"));
  assert(strcmp(buffer,"Este mensaje no tiene el separador")==0);

  strcat(buffer, "\n");
  bytesAcumulados=strlen(buffer);
  int numeroMensajesNuevo = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion);
  /* No debemos tener ningun mensaje procesado */
  assert(1==numeroMensajesNuevo);
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"NOT_IDENTIFIED\"}\n")==0);
}
