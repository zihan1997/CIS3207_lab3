#include <stdio.h>
#define MAX 20
struct log
{
    char word[20]; // the word need to be checked
    char status[20]; // the result of checking word
};

typedef struct{
    int size;
    struct log queue[MAX];
}queue_log;

typedef struct {
    int size;
    int queue[MAX];
}queue_job;

void init_queue_log(queue_log* log_queue){
    log_queue->size = 0;
}

void init_queue_job(queue_job* job_queue){
    job_queue->size = 0;
}

int enQueue_log(queue_log* log_queue, int socket){
    // queue is full
    if(log_queue->size == (MAX-1)){
        return 0;
    }
    int *arr = log_queue->queue;
    arr[log_queue->size] = socket;
    log_queue->size += 1;
    return 1;
}
int enQueue_job(queue_job* job_queue, int socket){
    // queue is full
    if(job_queue->size == (MAX-1)){
        return 0;
    }
    int *arr = job_queue->queue;
    arr[job_queue->size] = socket;
    job_queue->size += 1;
    return 1;
}
