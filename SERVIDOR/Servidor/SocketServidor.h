#ifndef SOCKETSERVIDOR_H
#define SOCKETSERVIDOR_H
#include <stdbool.h>
void runServidor();
void *atenderCliente(void *arg);
int procesarBuffer(char *buffer, int *bytesAcumulados, char **inicioMensaje, char *mensajes[50],bool *desconexion, int socket_fd);
#endif
