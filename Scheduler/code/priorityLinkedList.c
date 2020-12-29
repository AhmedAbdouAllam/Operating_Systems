#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>

struct Node
{
	int ID; 
	int priority;
	int arrivalTime;
	int runningTime;
	int priorityOfList;// this parameter will be used in priority LinkedList to be arranged
	struct Node *next;
	
};

// create new node 
struct Node* createNode(int id,int p,int at,int rt,int pof){
	struct Node* n = (struct Node*)malloc(sizeof(struct Node));
	n->ID=id;
	n->priority=p;
	n->arrivalTime=at;
	n->runningTime=rt;
	n->priorityOfList=pof;
	n->next=NULL;
	return n;
}

//head will point to highest priority ==> least value
//tail will point to lowest priority  ==> greatest value
 struct PriorityLinkedList{
 	struct Node* head;
 	struct Node* tail;
 	int count;
 };
 
 
 //create priority linked list and return pointer to it
 struct PriorityLinkedList* createPriorityLinkedList(){
    struct PriorityLinkedList* pll = (struct PriorityLinkedList*)malloc(sizeof(struct PriorityLinkedList)); 
    pll->head=NULL;
    pll->tail=NULL;
    pll->count=0;
    return pll;
 }
 
 
 void Insert(struct PriorityLinkedList* pll,int id,int p,int at,int rt,int pof)
 {
 	struct Node* n=createNode(id,p,at,rt,pof);
 	// if list is empty
 	if(pll->head == NULL)
 	{
 		pll->head=n;
    		pll->tail=n;
    		pll->count++;
    		return;
 	}
 	// tail has priority greater than node's priority
 	if(pll->tail->priorityOfList <= n->priorityOfList)//to apply stablility
 	{
 		pll->tail->next=n;
 		pll->tail=n;
 		pll->count++;
 	}
 	else if(pll->head->priorityOfList > n->priorityOfList)
 	{
 		n->next=pll->head;
 		pll->head=n;
 		pll->count++;
 	}
 	
 	else
 	{
 		struct Node* temp=pll->head;
 		while(temp!=NULL && temp->next!=NULL && temp->next->priorityOfList < n->priorityOfList)
 			temp=temp->next;
 		n->next=temp->next;
 		temp->next=n;
 		pll->count++;
 		
 	}
 	
 }
 //return node before wanted node
 struct Node* Search(struct PriorityLinkedList* pll,int id)
 {
 	if(pll->head == NULL)
 		return NULL;
 	struct Node* temp=pll->head;
 	if(temp->next->ID == id)
 		return pll->head;
 	for(int i=1;i<pll->count;i++)
 	{	
 		if(temp!=NULL && temp->next!=NULL&& temp->next->ID == id)
 		{	
 			return temp;
 		}
 		temp=temp->next;
 	}
 }
 
 
 bool Delete(struct PriorityLinkedList* pll,int id)
 {	bool flag =false;
 	if(pll->head == NULL)
 		return flag;
 	//delete first node in list
 	if(id==pll->head->ID)
 	{	struct Node* tempDeleted=pll->head;
 		pll->head=pll->head->next;
 		tempDeleted->next=NULL;
 		flag=true;
 		pll->count--;
 		free(tempDeleted);
 		return flag;
 	}
 	struct Node* temp1=Search(pll,id);
 	//printf("%d\n",temp1->ID);
 	struct Node* tempDeleted=temp1->next;
 	//printf("%d\n",tempDeleted->ID);
 	
 	//delete last node in list
 	 if(tempDeleted->next==NULL)
 	{
 		pll->tail=temp1;
 		pll->tail->next=NULL;
 		free(tempDeleted);
 		flag=true;
 		pll->count--;
 	}
 	
 	
 	else if(temp1==NULL)
 		return flag;
 	else
 	{
 		temp1->next=tempDeleted->next;
 		tempDeleted->next=NULL;
 		free(tempDeleted);
 		flag=true;
 		pll->count--;
 	}
 	return flag;
 	
 }
 

