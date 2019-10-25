#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int current;
    int sequence[10];
    int size;
} Node;

// make the array all zeros while initializing
void initializeQ(Node *queue)
{
    if(queue == NULL){
        return;
    }
    queue->current = 0;
    queue->size = 10;
    for (int i = 0; i < queue->size; i++)
    {
        (queue->sequence)[i] = 0;
    }
}

int enQueue(Node *queue, int jobSeq)
{
    int size = queue->size;
    int *seq = queue->sequence;
    // if full, return
    if (queue->current == size)
    {
        return 0;
    }
    // add new one
    seq[queue->current] = jobSeq;
    queue->current += 1;
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

int deQueue(Node *queue){
    if (queue == NULL){
        return 0;
    }
    int del = queue->sequence[0];
    queueAdjust(queue->sequence, &queue->current);
    return del;
}

// print the queue
void toString(Node *queue)
{
    for (int i = 0; i < queue->size; i++)
    {
        printf("Job:%d\t", queue->sequence[i]);
    }
    puts("");
}

// int main(void){
//     Node queue;
//     initializeQ(&queue);
//     enQueue(&queue, 1);
//     enQueue(&queue, 2);
//     enQueue(&queue, 3);
//     enQueue(&queue, 4);
//     enQueue(&queue, 5);
//     enQueue(&queue, 6);
//     enQueue(&queue, 7);
//     enQueue(&queue, 8);
//     enQueue(&queue, 9);
//     // enQueue(queue, 10);
//     enQueue(&queue, 11);
//     enQueue(&queue, 12);
//     enQueue(&queue, 13);

//     deQueue(&queue);
//     deQueue(&queue);
//     toString(&queue);
//     return 1;
// }