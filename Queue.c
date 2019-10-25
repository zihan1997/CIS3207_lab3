#include <stdio.h>
#define MAX 20
struct log
{
    char word[20]; // the word need to be checked
    char status[20]; // the result of checking word
};

typedef struct{
    int front;
    int tail;
    struct log queue[MAX];
}queue_log;

typedef struct {
    int front;
    int tail;
    int queue[MAX];
}queue_job;

void init_queue_log(queue_log* log_queue){
    log_queue->front = 0;
    log_queue->tail = 0;
}

void init_queue_job(queue_job* job_queue){
    job_queue->front = 0;
    job_queue->tail = 0;
}