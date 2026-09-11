#include <assert.h>
#include <string.h>
#include "Servidor/SocketServidor.h"

/* Prueba unitaria que reviza que ignora mensajes vacios \n\n */
void TestMensajeVacio(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[50]={0};
  char *inicioMensaje=buffer;
  char *mensajes[20];
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "hola\n\nadios\n");
  int bytesAcumulados=strlen(buffer);
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes);
  /* Aunque pasame 3 \n solo debe de guardar 2 mensajes  */
  assert(2==numeroMensajes);
  /* Revisemos el contenido de lo guardado coincide a lo esperado */
  assert(strcmp(mensajes[0],"bienvenido\n")==0);
  assert(strcmp(mensajes[1],"vuelve pronto\n")==0);
}

/* Prueba unitaria que reviza si se se guarda un mensaje que aun no tiene \n */
/* para despues unirlo cuando nos llegue otro mensaje */
void TestMensajeSinSeparador(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[50]={0};
  char *inicioMensaje=buffer;
  char *mensajes[20];
  strcpy(buffer, "Este mensaje no tiene el separador");
  int bytesAcumulados=strlen(buffer);
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes);
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
  char *mensajes[20];
  strcpy(buffer, "Este mensaje no tiene el separador");
  int bytesAcumulados=strlen(buffer);
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes);
  /* No debemos tener ningun mensaje procesado */
  assert(0==numeroMensajes);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == strlen("Este mensaje no tiene el separador"));
  assert(strcmp(buffer,"Este mensaje no tiene el separador")==0);

  strcat(buffer, "\n");
  bytesAcumulados=strlen(buffer);
  int numeroMensajesNuevo = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes);
  /* No debemos tener ningun mensaje procesado */
  assert(1==numeroMensajesNuevo);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == 0);
  assert(strcmp(buffer,"")==0);

}
