#ifndef SOCKETSERVIDOR_H
#define SOCKETSERVIDOR_H
#include <stdbool.h>
#include <stddef.h>      
#include <sys/types.h>

void runServidor();
void *atenderCliente(void *arg);
ssize_t escribirCompleto(int fd, const char *buf, size_t len);
int procesarBuffer(char *buffer, int *bytesAcumulados, char **inicioMensaje, char *mensajes[50],bool *desconexion, int socket_fd, bool *identificacion);
#endif
