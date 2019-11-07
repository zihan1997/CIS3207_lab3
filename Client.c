#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 10086

int main(int argc, char const *argv[])
{
    // Port get
    unsigned int port = DEFAULT_PORT;
    if(argc == 2){
        port = atoi(argv[1]);
    }
    // Socket implement
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        perror("failed to create socket\n");
        return -1;
    }
    // connect
    struct sockaddr_in serveradr;
    serveradr.sin_family = AF_INET;
    serveradr.sin_port = htons(port);
    
    // get binary form of IP
    if( inet_pton(AF_INET, "127.0.0.1", &serveradr.sin_addr) <= 0 ){
        perror("Invalid IP addr\n");
        return -1;
    }

    if( connect(clientSocket, (struct sockaddr *)&serveradr, sizeof(serveradr)) < 0){
        perror("failed to connet\n");
        return -1;
    }
    char buffer[BUFFER_SIZE] = "\0";
    while(1){
        // fflush(stdin);
        // fflush(stdout);
        printf("Word> ");
        scanf("%s", buffer);
        // escape char
        if(buffer[0] == 27){
            close(clientSocket);
            break;
        }
        if(send(clientSocket, buffer, sizeof(buffer), 0) < 0){
            printf("send failed\n");
        }
        recv(clientSocket, buffer, sizeof(buffer), 0);
        // printf("received buffer:\n%s\n", buffer);
        printf("%s\n", buffer);
        
    }
    return 0;
}
