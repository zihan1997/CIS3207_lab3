#include <stdio.h>
#include <string.h>
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
#define MAX_WORKERS 10


queue_log results;
pthread_mutex_t mutex;
pthread_cond_t fill, empty;

unsigned int port;

void* clients(void* id){
    // common struct
    struct sockaddr_in serveradr;
    serveradr.sin_family = AF_INET;
    serveradr.sin_port = htons(port);

    // get binary form of IP
    if( inet_pton(AF_INET, "127.0.0.1", &serveradr.sin_addr) <= 0 ){
        perror("Invalid IP addr\n");
        return 0;
    }

    while(1){
        pthread_mutex_lock(&mutex);
        while(results.size == 0){
            pthread_cond_wait(&fill, &mutex);
        }
        printf("ID> %d\n", *(int *)id);
        struct log w;
        deQueue_log(&results, &w);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

        int sockets = socket(AF_INET, SOCK_STREAM, 0);
        if(sockets < 0){
            perror("failed to create socket\n");
            return 0;
        }
        // get binary form of IP
        if( inet_pton(AF_INET, "127.0.0.1", &serveradr.sin_addr) <= 0 ){
            perror("Invalid IP addr\n");
            return 0;
        }
        if( connect(sockets, (struct sockaddr *)&serveradr, sizeof(serveradr)) < 0){
            perror("failed to connet\n");
            return 0;
        }
        // For debug
            // puts("connected");
            // printf("Sending word %s\n", w.word);
        send(sockets, w.word, sizeof(w.word), 0);
        char buffer[BUFFER_SIZE] = "\0";
        recv(sockets, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
        puts("finished");

        // Close connect
        char exitMsg[2];
        exitMsg[0] = 27;
        exitMsg[1] = '\0';
        send(sockets, exitMsg, sizeof(exitMsg), 0);
        close(sockets);
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    // Port get
    port = DEFAULT_PORT;
    if(argc == 2){
        port = atoi(argv[1]);
    }
    // Initial Queues
    init_queue_log(&results);

    // Initial cond_t mutex
    pthread_cond_init(&empty, 0);
    pthread_cond_init(&fill, 0);
    pthread_mutex_init(&mutex, 0);

    // Threads
    pthread_t threads[MAX_WORKERS];
    int ids[MAX_WORKERS];
    for(int i = 0; i < MAX_WORKERS; i++){
        ids[i] = i;
        pthread_create(&threads[i], NULL, &clients, &ids[i]);
    }

    puts("finish init");
    char word[20];
    do{
        sleep(1);
        printf("> ");
        scanf("%s", word);
        if(word[0] == 27){
            break;
        }
        pthread_mutex_lock(&mutex);
        while(results.size == MAX){
            pthread_cond_wait(&empty, &mutex);
        }
        struct log w;
        strcpy(w.word, word);
        strcpy(w.status, "null");
        enQueue_log(&results, w);
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    }while(word[0] != 27);

    return 0;
}
