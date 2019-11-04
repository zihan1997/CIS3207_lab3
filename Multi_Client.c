#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Queue.c"

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 10086

void* clients(void* id){
    return 1;
}

int main(int argc, char const *argv[])
{
    // Initial Queues
    queue_job connects;
    init_queue_job(&connects);
    
    return 0;
}
