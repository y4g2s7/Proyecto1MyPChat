#include <assert.h>
#include <string.h>
#include "Servidor/SocketServidor.h"

void TestMensajeVacio(){
  char buffer[50]={0};
  
  char *inicioMensaje=buffer;
  char *mensajes[20];
  strcpy(buffer, "hola\n\nadios\n");
  int bytesAcumulados=strlen(buffer);
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes);
  assert(2==numeroMensajes);
}

int main() {
  TestMensajeVacio();
  return 0;
}

