#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Queue.c"
#include "open_listenfd.c"

// Constants
// port between 1024 to 65535
#define DEFAULT_PORT 10086
#define DEFAULT_DICTIONARY "words.txt"
#define MAX_WORKER 20
#define BUFFER_SIZE 1024

FILE *logFile;
FILE *dict;

queue_job job_queue;
queue_log log_queue;

// empty: jsut dequeued, ready to enqueue more
// fill: new connect comes in
pthread_cond_t empty, fill;
pthread_mutex_t mutex, mutex_log;

// get job queue, check words and send to log queue
void* worker_thread(void* id){
    // printf("Thread>%d\n", *(int *)id);
    // fprintf(logFile,"Thread>%d\n", *(int *)id);
    
    int client = 0;
    //Get the socket discriptor
    pthread_mutex_lock(&mutex);
    while(job_queue.size == 0){
        pthread_cond_wait(&fill, &mutex);
    }
    deQueue_job(&job_queue, &client);
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);

    printf("Thread>%d\n", *(int *)id);

    char buffer[BUFFER_SIZE] = "\0";
    // Start to receive data
    while( recv(client, buffer, sizeof(buffer), 0) > 0){
        if(buffer[0] == 27){
            break;
        }
        struct log result = {"\0", "\0"};
        strcpy(result.word, buffer);

        // Check the word
        fflush(dict);
        char word[100];
        while(fscanf(dict, "%s", word) != EOF){
            // Match the word
            printf("Word: %s, Buffer: %s\n", word, buffer);
            if(strcmp(word, buffer) == 0){
                // Enqueue the result of a word
                pthread_mutex_lock(&mutex_log);
                while(log_queue.size == MAX){
                    pthread_cond_wait(&empty, &mutex_log);
                }
                strcpy(result.status, "OK\n");
                printf("\nCorrect\n");
                enQueue_log(&log_queue, &result);
                pthread_mutex_unlock(&mutex_log);
                break;
            }else{
                strcpy(result.status, "MISSPELLED\n");
            }
        }
        printf("The word: %s > %s\n", result.word, result.status);
        // Send result to client
        send(client, result.status, sizeof(result.status), 0);
    }

    return 0;
}


int main(int argc, char const *argv[])
{

    // Initial Queues
    init_queue_job(&job_queue);
    init_queue_log(&log_queue);
    
    //* Open files
    logFile = fopen("log.txt", "w");
    if(logFile == NULL){
        printf("lologFileg failed to open\n");
        // when failed to open, ouput to screen
        logFile = stdout;
    }
    fprintf(logFile, "Log file opened.\n");
    // decide port and dictionary
    // Order: Command Port Dictionary
    unsigned int port = DEFAULT_PORT;
    static char dictionary[50] = DEFAULT_DICTIONARY;
    // both provided
    if(argc == 3){
        strcpy(dictionary, argv[2]);
        port = atoi(argv[1]);
    }
    // one of them provided
    else if(argc == 2){
        // it is dict
        if(strstr(argv[1], "txt")){
            strcpy(dictionary, argv[1]);
        }
        // it is port
        else{
            port = atoi(argv[1]);
        }
    }else{
        ;// go default
    }
    // printf("Port: %d\nDict: %s\n", port, dictionary);
    //* Read dict
    dict = fopen(dictionary, "r");
    if(!dict){
        printf("log failed to open\n");
        // if not readable, open default one
        dict = fopen(DEFAULT_DICTIONARY, "r");
    }
    fprintf(logFile, "Dict file %s read.\n", dictionary);

    //* Create threads

    // initialize mutext and cond
    pthread_cond_init(&empty, 0);
    pthread_cond_init(&fill, 0);
    pthread_mutex_init(&mutex, 0);

    // ids array is to assign id to each thread
    int ids[MAX_WORKER];
    pthread_t threads[MAX_WORKER];
    for(int i = 0; i < MAX_WORKER; i++){
        ids[i] = i;
        // printf("id: %d\n", ids[i]);
        pthread_create(&(threads[i]), NULL, &worker_thread, &(ids[i]));
    }

    // for(int i = 0; i < MAX_WORKER; i++){
    //     pthread_join(threads[i], NULL);
    // }
    // puts("All Done");



    // Socket implement
    struct sockaddr_in client;
    int clientLen = sizeof(client);
    int connectionSocket, clientSocket;
    char recvBuffer[1024] = "\0";

    if(port < 1024 || port > 65535){
        printf("port needs to be between 1024 and 65535\n");
        printf("Due to the error, port is set to default one: %d", 
                                                        DEFAULT_PORT);
    }
    connectionSocket = open_listenfd(port);
    if(connectionSocket == -1){
        printf("Error occured connecting to %d\n", port);
        return -1;
    }
    while (1)
    {
        // if connection failed, go to next loop to wait
        clientSocket = accept(connectionSocket, 0, 0);
        if(clientSocket == -1){
            printf("connection failed\n");
            continue;
        }
        // recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        // printf("The words: %s\n", recvBuffer);
        // char result[20] = "OK";
        // send(clientSocket, result, sizeof(result), 0);
        pthread_mutex_lock(&mutex);
        enQueue_job(&job_queue, clientSocket);
        while(job_queue.size == MAX){
            pthread_cond_wait(&empty, &mutex);
        }
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}
