
#include <stdio.h> 
#include <stdlib.h> 
#include "headers.h"

  
// A linked list (LL) node to store a queue entry 
struct QNode
{
	struct PCB process_control_block;
	struct QNode *next;
	//struct node *Back;
};
  
// The queue, front stores the front node of LL and rear stores the 
// last node of LL 
struct Queue { 
    struct QNode *front, *rear; 
}; 
  
// A utility function to create a new linked list node. 
struct QNode* newNode(struct PCB p) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->process_control_block=p; 
    temp->next = NULL; 
    return temp; 
} 
  
// A utility function to create an empty queue 
struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void enQueue(struct Queue* q, struct PCB p) 
{ 
    // Create a new LL node 
    struct QNode* temp = newNode( p); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
// Function to remove a key from given queue q 
struct PCB deQueue(struct Queue* q) 
{ 
    // If queue is empty, return NULL. 
  //  if (q->front == NULL) 
    //    return 0; 
  
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
   
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
  
    free(temp); 
    return temp->process_control_block;
} 
  
// Driver Program to test anove functions 
/*int main() 
{ 
   /* struct Queue* q = createQueue(); 
    struct PCB p;
    /p.Process_ID=0;
    truct PCB p1;
    p1.Process_ID=1;
    struct PCB p2;
    p2.Process_ID=2;
    struct PCB p3;
    p3.Process_ID=3;
    struct PCB p4;
    p4.Process_ID=4;
    enQueue(q, p); 
    enQueue(q, p1); 
    deQueue(q); 
    deQueue(q); 
    enQueue(q, p2); 
    enQueue(q, p3); 
    enQueue(q, p4); 
    deQueue(q); 
    printf("Queue Front : %d \n", q->front->process_control_block.Process_ID); 
    printf("Queue Rear : %d", q->rear->process_control_block.Process_ID);}*/
    
    
