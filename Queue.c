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

int enQueue_log(queue_log* log_queue, struct log result){
    // queue is full
    if(log_queue->size == (MAX-1)){
        return 0;
    }
    // int *arr = log_queue->queue;
    // arr[log_queue->size] = result;
    log_queue->queue[log_queue->size] = result;
    log_queue->size += 1;
    return 1;
}
int enQueue_job(queue_job* job_queue, int socket){
    // queue is full
    if(job_queue->size == (MAX-1)){
        return 0;
    }
    // int *arr = job_queue->queue;
    // arr[job_queue->size] = socket;
    (job_queue->queue)[job_queue->size] = socket;
    job_queue->size += 1;
    return 1;
}

void queueAdjust(int *arr, int *size)
{
    for (int i = 0; i < *size - 1; i++)
    {
        arr[i] = arr[i + 1];
    }
    // make the last one zero
    arr[*size - 1] = 0;
    // current size - 1
    *size -= 1;
}

int deQueue_log(queue_log* log_queue, struct log *result){
    // check if the queue if empty
    if(log_queue->size == 0){
        return 0;
    }else{
        // int *arr = log_queue->queue;
        *result = log_queue->queue[0];
        for(int i = 0; i < log_queue->size - 1; i++){
            log_queue->queue[i] = log_queue->queue[i+1];
        }
        log_queue->size-=1;
        return 1;
    }
}

int deQueue_job(queue_job* job_queue, int *socket){
    // check if the queue if empty
    if(job_queue->size == 0){
        return 0;
    }else{
        int *arr = job_queue->queue;
        *socket = arr[0];
        queueAdjust(arr, &job_queue->size);
        return 1;
    }
}
void printQueue(int *arr, int size){
    for(int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    puts("");
}

/*
int main(int argc, char const *argv[])
{
    struct log result0 = {"0", "ok"};
    struct log result1 = {"1", "ok"};
    struct log result2 = {"2", "ok"};
    struct log result3 = {"3", "ok"};
    queue_log logs;
    struct log result;
    init_queue_log(&logs);
    enQueue_log(&logs, result0);
    enQueue_log(&logs, result1);
    enQueue_log(&logs, result2);
    enQueue_log(&logs, result3);
    deQueue_log(&logs, &result);
    deQueue_log(&logs, &result);
    for(int i = 0; i < logs.size; i++){
        printf("%d. %s>%s\n", i, logs.queue[i].word, logs.queue[i].status);
    }
    printf(" %s>%s\n", result.word, result.status);
    return 0;
}
*/