#ifndef SOCKETSERVIDOR_H
#define SOCKETSERVIDOR_H
void runServidor();
void *atenderCliente(void *arg);
int procesarBuffer(char *buffer, int *bytesAcumulados, char **inicioMensaje, char *mensajes[20]);
#endif
