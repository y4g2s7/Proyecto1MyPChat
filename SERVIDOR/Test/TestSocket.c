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
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  /* Aunque pasemos 3 "\n" solo debe de guardar 2 mensajes  */
  assert(2==numeroMensajes);
  /* Revisemos el contenido de lo guardado coincide a lo esperado */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Kimberly\"}\n")==0);
  assert(strcmp(mensajes[1],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"USER_ALREADY_EXISTS\",\"extra\":\"Kimberly\"}\n")==0);  
}


/* Prueba unitaria que reviza si se detecta los username duplicado */
void TestNombreDuplicado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes y regresar el json correspondiente*/
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  strcpy(buffer, "{\"type\":\"IDENTIFY\",\"username\":\"Juan Gabriel\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  assert(1==numeroMensajes);
  /* Revisemos el contenido de lo guardado es que el usuario se pudo identificar bien */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Juan Gab\"}\n")==0);
  

  char buffer2[100]={0};
  char *inicioMensaje2=buffer2;
  char *mensajes2[50];
  /* Mensaje con el mismo username ya registrado */
  strcpy(buffer2, "{\"type\":\"IDENTIFY\",\"username\":\"Juan Gabriel\"}\n");
  int bytesAcumulados2=strlen(buffer2);
  bool desconexion2 = false;
  int numeroMensajes2 = procesarBuffer(buffer2,&bytesAcumulados2,&inicioMensaje2,mensajes2,&desconexion2,3);
 
  assert(1==numeroMensajes2);
  /* Revisemos el contenido de lo guardado dice que el username ya se uso*/ 
  assert(strcmp(mensajes2[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"USER_ALREADY_EXISTS\",\"extra\":\"Juan Gab\"}\n")==0);
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
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes, &desconexion,2);
  /* No debemos tener ningun mensaje procesado */
  assert(0==numeroMensajes);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == strlen("Este mensaje no tiene el separador"));
  assert(strcmp(buffer,"Este mensaje no tiene el separador")==0);
}

void TestMensajeSinSeparadorCompletado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes y regresar el json */
  char buffer[50]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  strcpy(buffer, "Este mensaje no tiene el separador");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  /* No debemos tener ningun mensaje procesado */
  assert(0==numeroMensajes);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == strlen("Este mensaje no tiene el separador"));
  assert(strcmp(buffer,"Este mensaje no tiene el separador")==0);

  strcat(buffer, "\n");
  bytesAcumulados=strlen(buffer);
  int numeroMensajesNuevo = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  /* Ahora si tenemos un mensaje */
  assert(1==numeroMensajesNuevo);
  /* Como no cumple con el protocolo vemos si regresa lo esperado en este caso */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"NOT_IDENTIFIED\"}\n")==0);
}


/* Prueba unitaria que reviza que se cambie de estatus */
void TestCambioEstatus(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"STATUS\",\"status\":\"AWAY\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  /* No debemos tener ningun mensaje porque al cambiar de estado no avisa al cliente nada  */
  assert(0==numeroMensajes);  
}

/* Prueba unitaria que reviza que no se cambia de estatus cuando no cumple el protocolo */
void TestCambioEstatusNoAceptable(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"STATUS\",\"status\":\"ESTATUSLOCO\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  /* Debemos tener un mensaje, el que manda el servidor cuando no cumple el protocolo  */
  assert(1==numeroMensajes);  
  /*Como no cumple con el protocolo vemos si regresa lo esperado en este caso */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"NOT_IDENTIFIED\"}\n")==0);
}

/* Prueba unitaria que reviza si el cambio de estado es el mismo que ya tenia es fuera del protocolo */
void TestCambioEstatusRepetido(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"STATUS\",\"status\":\"AWAY\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2);
  /* Debemos tener un mensaje, el que manda el servidor cuando no cumple el protocolo  */
  assert(1==numeroMensajes);  
  /*Como no cumple con el protocolo vemos si regresa lo esperado en este caso */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"NOT_IDENTIFIED\"}\n")==0);
}
