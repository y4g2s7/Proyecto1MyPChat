#ifndef INDICACIONESSERVIDOR_H
#define INDICACIONESSERVIDOR_H
#include <cjson/cJSON.h>
#include <stdbool.h>
char *traduccionJSON(char *mensaje, int socket_fd, bool *identificacion);
char *invitarSala(cJSON *nombreSalaNodo, cJSON *usuariosNodo,int socket_fd);
char *sala(cJSON *nombreSalaNodo, int socket_fd);
char *mensajePublico(cJSON *mensajePublicoNodo, int socket_fd);
char *mensajePrivado(cJSON *destinatario, cJSON *mensaje, int socket_fd);
char *getUsername(int socket_fd);
char *cambioEstado(cJSON *statusNodo, int socket_fd);
char *Identificar(cJSON *usernameNodo, int socket_fd, bool *identificacion);
#endif
