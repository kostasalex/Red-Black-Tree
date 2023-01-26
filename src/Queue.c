#include <stdio.h>
#include <stdlib.h>
#include "QItem.h"

typedef struct Queue Q;
struct Queue{
    Q *next;
    QItem item;
}*front, *rear;

void CreateQueue(void){
    front = rear = NULL;
}

int QEmpty(void){
   return(front == NULL);
}

void EnQueue(QItem item){
    Q *new;
    
    if((new = malloc(sizeof(Q))) == NULL){
        printf("Memory problem , try again..");
        exit(-1);
    }  
    new->item = item;
    new->next  = NULL;
    /*Case 1: Queue is empty.*/
    if(QEmpty())
        front = rear = new;
    /*Case 2: Queue has at least 1 node.*/
    else{
        rear->next = new;
        rear = new;
    }
}

QItem DeQueue(void){
    Q *temp = front;        
      
    QItem item = front->item; 
    front = front->next;      
    free(temp);                   

    return item;
}
