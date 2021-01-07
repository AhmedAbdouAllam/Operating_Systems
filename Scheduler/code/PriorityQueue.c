	#include <stdio.h> 
	#include <stdlib.h> 
	#include "headers.h"

	// A linked list (LL) node to store a queue entry 
	struct PQNode
	{
		struct PCB Process_Control_Block;
		struct PQNode *next;
		int priorityQ;// this parameter will be used to arrange queue according to
	};
	  
	// The priority queue, front stores the front node with highest priority (least value) of LL and rear stores the 
	// last node of LL with lowest priority (highest value)
	struct PriorityQueue { 
		struct PQNode *front, *rear; 
	}; 
	  
	// A utility function to create a new linked list node. 
	struct PQNode* newPQNode(struct PCB P,int priority1) 
	{ 
		struct PQNode* temp = (struct PQNode*)malloc(sizeof(struct PQNode)); 
		temp->Process_Control_Block = P; 
		temp->priorityQ=priority1;
		temp->next = NULL; 
		return temp; 
	} 
	  
	// A utility function to create an empty queue 
	struct PriorityQueue* createPriorityQueue() 
	{ 
		struct PriorityQueue* q = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue)); 
		q->front = q->rear = NULL; 
		return q; 
	} 

	void PriorityenQueue(struct PriorityQueue* q,struct PCB P,int priorityq) 
	{ 
		// Create a new LL node 
		struct PQNode* temp = newPQNode(P,priorityq); 
	  
		// If queue is empty, then new node is front and rear both 
		if (q->rear == NULL) { 
		    q->front = q->rear = temp; 
		    return; 
		} 
	  
		// tail has priority greater than node's priority
	 	if(q->rear->priorityQ <= temp->priorityQ)//to apply stablility
	 	{
	 		q->rear->next=temp;
	 		q->rear=temp;
	 		
	 	}
	 	else if(q->front->priorityQ > temp->priorityQ)
	 	{
	 		temp->next=q->front;
	 		q->front=temp;
	 	}
	 	
	 	else
	 	{
	 		struct PQNode* tempPtr=q->front;
	 		while(tempPtr!=NULL && tempPtr->next!=NULL && tempPtr->next->priorityQ <= temp->priorityQ)
	 			tempPtr=tempPtr->next;
	 		temp->next=tempPtr->next;
	 		tempPtr->next=temp;
	 
	 		
	 	}
	} 
	  
	// Function to remove a key from given queue q 
	void PrioritydeQueue(struct PriorityQueue* q) 
	{ 
		// If queue is empty, return NULL. 
		if (q->front == NULL) 
		    return; 
	  
		// Store previous front and move front one node ahead 
		struct PQNode* temp = q->front; 
	  
		q->front = q->front->next; 
	  
		// If front becomes NULL, then change rear also as NULL 
		if (q->front == NULL) 
		    q->rear = NULL; 
	  
		free(temp); 
	}
	
	
	
	 /*
	 // FOR TESTING ONLY
	 //
	 void printList(struct PriorityQueue* pll)
	 {

	 	struct PQNode* temp=pll->front;
	 	if(temp==NULL)
	 		printf("EMPTY \n");
	 	while(temp!=NULL)
	 	{
	 		printf("%d  %d\n",temp->Process_Control_Block.Proc.Process_ID,temp->priorityQ);
	 		temp=temp->next;

	 	}
	 	printf("*********************\n");

	 }

	// testing utilities
	int main() 
	{ 
		struct PCB n1;
		n1.Proc.Process_ID=1;
		printf("%d\n",n1.Proc.Process_ID);
		struct PCB n2;
		n2.Proc.Process_ID=2;
		struct PCB n3;
		n3.Proc.Process_ID=3;
		struct PCB n4;
		n4.Proc.Process_ID=4;
		struct PCB n5;
		n5.Proc.Process_ID=5;
		struct PCB n6;
		n6.Proc.Process_ID=6;
		struct PriorityQueue* p = createPriorityQueue();
		PriorityenQueue(p,n1,5);
		PriorityenQueue(p,n2,4);
		PriorityenQueue(p,n3,20);
		PriorityenQueue(p,n4,8);
		PriorityenQueue(p,n5,7);
		PriorityenQueue(p,n6,5);
		printList(p);
		printf("%d  %d\n",p->front->Process_Control_Block.Proc.Process_ID,p->rear->Process_Control_Block.Proc.Process_ID);
		
		PrioritydeQueue(p);
		   printList(p);

		PrioritydeQueue(p);
		   printList(p);
		   PrioritydeQueue(p);
		   printList(p);
		   PrioritydeQueue(p);
		   printList(p);
		   PrioritydeQueue(p);
		   printList(p);
		   PrioritydeQueue(p);
		   printList(p);
		return 0; 
	}
	*/
