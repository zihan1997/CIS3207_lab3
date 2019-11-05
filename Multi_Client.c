#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
// socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Queue.c"

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 10086

pthread_mutex_t mutex;
pthread_cond_t fill, empty;

unsigned int port;

void* clients(void* id){
    printf("Thread%d ready\n", *(int *)id);

    while(1){

        pthread_mutex_lock(&mutex);
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
            fflush(stdin);
            fflush(stdout);
            printf("Word> ");
            scanf("%s", buffer);
            send(clientSocket, buffer, sizeof(buffer), 0);
            // escape char
            if(buffer[0] == 27){
                close(clientSocket);
                break;
            }
            recv(clientSocket, buffer, sizeof(buffer), 0);
            // printf("received buffer:\n%s\n", buffer);
            printf("%s\n", buffer);
            
        }
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    // Initial Queues
    queue_job connects;
    init_queue_job(&connects);

    // Initial cond_t mutex
    pthread_cond_init(&empty, 0);
    pthread_cond_init(&fill, 0);
    pthread_mutex_init(&mutex, 0);

    // Port get
    port = DEFAULT_PORT;
    if(argc == 2){
        port = atoi(argv[1]);
    }
    // Socket implement
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        perror("failed to create socket\n");
        return -1;
    }
    
    
    
    return 0;
}
