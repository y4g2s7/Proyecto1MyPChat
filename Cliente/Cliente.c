#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in direccion = {0};
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &direccion.sin_addr);
    
    connect(socket_fd, (struct sockaddr *)&direccion, sizeof(direccion));

    char *mensaje = "juan";
    write(socket_fd, mensaje, strlen(mensaje));

    /* char *m = "TEST"; */
    /* write(socket_fd, m, strlen(m)); */
    
    char buffer[256] = {0};
    ssize_t bytes_leidos;
    while((bytes_leidos = read(socket_fd, buffer, sizeof(buffer)))>0){
      printf("El servidor respondió: %.*s\n", (int)bytes_leidos, buffer);
      
    }
    close(socket_fd);
    return 0;
} 
