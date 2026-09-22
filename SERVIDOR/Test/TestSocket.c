#include <assert.h>
#include <string.h>
#include "Servidor/SocketServidor.h"
#include <stdbool.h>
#include <stdio.h>

/* Prueba unitaria que reviza que ignora mensajes vacios \n\n */
void TestMensajeVacio(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion =false;
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"IDENTIFY\",\"username\":\"Kimberly\"}\n\n{\"type\":\"IDENTIFY\",\"username\":\"Kimberly\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2,&identificacion);
  /* Aunque pasemos 3 "\n" solo debe de guardar 2 mensajes  */
  assert(2==numeroMensajes);
  /* Revisemos el contenido de lo guardado coincide a lo esperado */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Kimberly\"}\n")==0);
  assert(strcmp(mensajes[1],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"INVALID\"}\n")==0);
}


/* Prueba unitaria que reviza si se detecta los socket duplicados */
void TestSocketDuplicado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes y regresar el json correspondiente*/
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion = true;
  strcpy(buffer, "{\"type\":\"IDENTIFY\",\"username\":\"Juan Gabriel\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2,&identificacion);
  assert(1==numeroMensajes);
  /* Revisemos el contenido de lo guardado es que el usuario no se pudo identificar y el servidor contesto error */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"INVALID\"}\n")==0);

  
  char buffer2[100]={0};
  char *inicioMensaje2=buffer2;
  char *mensajes2[50];
  bool identificacion2 =false;
  /* Nos identificamos desde un socket que no ha sido usado */
  strcpy(buffer2, "{\"type\":\"IDENTIFY\",\"username\":\"Juan Gabriel\"}\n");
  int bytesAcumulados2=strlen(buffer2);
  bool desconexion2 = false;
  int numeroMensajes2 = procesarBuffer(buffer2,&bytesAcumulados2,&inicioMensaje2,mensajes2,&desconexion2,3,&identificacion2);
 
  assert(1==numeroMensajes2);
  /* Ahora si podemos registrarnos con ese nombre*/ 
  assert(strcmp(mensajes2[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Juan Gab\"}\n")==0);
}

void TestNombreDuplicado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes y regresar el json correspondiente*/
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion =false;
  strcpy(buffer, "{\"type\":\"IDENTIFY\",\"username\":\"Luis Miguel\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,10,&identificacion);
  assert(1==numeroMensajes);
  /* Revisemos el contenido de lo guardado es que el usuario se pudo identificar bien */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Luis Mig\"}\n")==0);
  
  char buffer2[100]={0};
  char *inicioMensaje2=buffer2;
  char *mensajes2[50];
  bool identificacion2 =false;
  /* Repetimos el mismo username desde otro socket */
  strcpy(buffer2, "{\"type\":\"IDENTIFY\",\"username\":\"Luis Miguel\"}\n");
  int bytesAcumulados2=strlen(buffer2);
  bool desconexion2 = false;
  int numeroMensajes2 = procesarBuffer(buffer2,&bytesAcumulados2,&inicioMensaje2,mensajes2,&desconexion2,11,&identificacion2);
 
  assert(1==numeroMensajes2);
  /* No podemos registrarnos con ese username */ 

  /* printf("%s\n",mensajes2[0]); */
  /* assert(strcmp(mensajes2[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"NOT_IDENTIFIED\"}\n")==0); */
  assert(strcmp(mensajes2[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"USER_ALREADY_EXISTS\",\"extra\":\"Luis Mig\"}\n")==0);
}

/* Prueba unitaria que reviza si se se guarda un mensaje que aun no tiene \n */
/* para despues unirlo cuando nos llegue otro mensaje */
void TestMensajeSinSeparador(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[50]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion =false;
  strcpy(buffer, "Este mensaje no tiene el separador");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes, &desconexion,2,&identificacion);
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
  bool identificacion =true;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2, &identificacion);
  /* No debemos tener ningun mensaje procesado */
  assert(0==numeroMensajes);
  /* Pero si debemos tener el registro de el mensaje para despues procesarlo */
  assert(bytesAcumulados == strlen("Este mensaje no tiene el separador"));
  assert(strcmp(buffer,"Este mensaje no tiene el separador")==0);

  strcat(buffer, "\n");
  bytesAcumulados=strlen(buffer);
  int numeroMensajesNuevo = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2,&identificacion);
  /* Ahora si tenemos un mensaje */
  assert(1==numeroMensajesNuevo);
  /* Como no cumple con el protocolo vemos si regresa lo esperado en este caso */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"INVALID\"}\n")==0);
}


/* Prueba unitaria que reviza que se cambie de estatus */
void TestCambioEstatus(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion =true;
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"STATUS\",\"status\":\"AWAY\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = true;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2,&identificacion);
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
  bool identificacion =true;
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"STATUS\",\"status\":\"ESTATUSLOCO\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2,&identificacion);
  /* Debemos tener un mensaje, el que manda el servidor cuando no cumple el protocolo  */
  assert(1==numeroMensajes);  
  /*Como no cumple con el protocolo vemos si regresa lo esperado en este caso */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"INVALID\"}\n")==0);
}

/* Prueba unitaria que reviza si el cambio de estado es el mismo que ya tenia es fuera del protocolo */
void TestCambioEstatusRepetido(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion =true;
  /* Mensaje que incluye uno vacio */
  strcpy(buffer, "{\"type\":\"STATUS\",\"status\":\"AWAY\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,2, &identificacion);
  /* Debemos tener un mensaje, el que manda el servidor cuando no cumple el protocolo  */
  assert(1==numeroMensajes);  
  /*Como no cumple con el protocolo vemos si regresa lo esperado en este caso */
  assert(strcmp(mensajes[0],"{\"type\":\"RESPONSE\",\"operation\":\"INVALID\",\"result\":\"INVALID\"}\n")==0);
}

void TestMensajePrivado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer[100]={0};
  char *inicioMensaje=buffer;
  char *mensajes[50];
  bool identificacion =false;
  /* Nos identificamos */
  strcpy(buffer, "{\"type\":\"IDENTIFY\",\"username\":\"Ckan\"}\n");
  int bytesAcumulados=strlen(buffer);
  bool desconexion = false;
  int numeroMensajes = procesarBuffer(buffer,&bytesAcumulados,&inicioMensaje,mensajes,&desconexion,5,&identificacion);
  assert(1==numeroMensajes);
  
  char buffer2[1000]={0};
  char *inicioMensaje2=buffer2;
  char *mensajes2[50];
  bool identificacion2 =false;
  /* Nos identificamos desde otro coket y con otro nombre de usuario y mandamos un mensaje a otro ususario */
  strcpy(buffer2, "{\"type\":\"IDENTIFY\",\"username\":\"Babo\"}\n{\"type\":\"TEXT\",\"username\":\"Ckan\",\"text\":\"Perra, soy el Babo, tú nada mas eres Ckan\"}");
  int bytesAcumulados2=strlen(buffer2);
  bool desconexion2 = false;
  int numeroMensajes2 = procesarBuffer(buffer2,&bytesAcumulados2,&inicioMensaje2,mensajes2,&desconexion2,6,&identificacion2);
  /* Como enviar mensaje no regresa nada solo hay 1 mensaje */
  assert(1==numeroMensajes2);
  assert(strcmp(mensajes2[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Babo\"}\n")==0); 
}

void TestMensajeUsuarioNoEncontrado(){
  /* Hacemos las variables para poder llamar a procesarBuffer() que es la encargada de */
  /* separar los mensajes */
  char buffer2[1000]={0};
  char *inicioMensaje2=buffer2;
  char *mensajes2[50];
  bool identificacion =false;
  strcpy(buffer2, "{\"type\":\"IDENTIFY\",\"username\":\"Alex\"}\n{\"type\":\"TEXT\",\"username\":\"ABCD\",\"text\":\"Perra, soy el Babo, tú nada mas eres Ckan\"}\n");
  int bytesAcumulados2=strlen(buffer2);
  bool desconexion2 = false;
  int numeroMensajes2 = procesarBuffer(buffer2,&bytesAcumulados2,&inicioMensaje2,mensajes2,&desconexion2,7,&identificacion);
  /* Ahora si debe haber dos mensajes, identificacion y usuario no encontrado */
  assert(2==numeroMensajes2);
  assert(strcmp(mensajes2[0],"{\"type\":\"RESPONSE\",\"operation\":\"IDENTIFY\",\"result\":\"SUCCESS\",\"extra\":\"Alex\"}\n")==0); 
  assert(strcmp(mensajes2[1],"{\"type\":\"RESPONSE\",\"operation\":\"TEXT\",\"result\":\"NO_SUCH_USER\",\"extra\":\"ABCD\"}\n")==0);
}
