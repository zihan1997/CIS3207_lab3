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

int deQueue_log(queue_log* log_queue, int *socket){
    // check if the queue if empty
    if(log_queue->size == 0){
        return 0;
    }else{
        int *arr = log_queue->queue;
        *socket = arr[0];
        queueAdjust(arr, &log_queue->size);
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
    queue_job job_queue;
    init_queue_job(&job_queue);
    int num = 0;
    while(num != -1){
        printf("Interger read> ");
        scanf("%d", &num);
        if(num == -1){
            break;
        }
        int *arr = job_queue.queue;
        // printf("%d\n", num);
        if(num == 1){
            printf("enqueue> ");
            scanf("%d", &num);
            if(num == -1){
                break;
            }
            enQueue_job(&job_queue, num);
            printQueue(arr, job_queue.size);
        }
        if(num == 2){
            printf("dequeue> ");
            deQueue_job(&job_queue, &num);
            puts("");
            printf("dequeued: %d\n", num);
            int *arr = job_queue.queue;
            printQueue(arr, job_queue.size);
        }
    }
    return 0;
}
*/