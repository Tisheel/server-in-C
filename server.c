#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>

#include <unistd.h>

int main(){
    // port 8080
    struct sockaddr_in addr = {AF_INET, 0x901f, 0};

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1){
        printf("Cannot create server"); 
        exit(1);
    }

    int server_bind = bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    if(server_bind == -1){
        printf("Cannot bind the server.");
        exit(1);
    }

    // max client 10
    int server_listen = listen(server_fd, 10);

    if(server_listen == -1){
        printf("Cannot listen the server.");
        exit(1);
    }

    int client_fd = accept(server_fd, 0, 0);

    if(client_fd == -1){
        printf("Cannot accept client.");
        exit(1);
    }

    char buffer[256] = {0};

    recv(client_fd, buffer, 256, 0);

    // GET /index.html ...
    char *fptr = buffer + 5;
    *strchr(fptr, ' ') = '\0';

    int html_fd = open(fptr, O_RDONLY);

    sendfile(client_fd, html_fd, 0, 256);

    close(html_fd);
    close(client_fd);
    close(server_fd);
}