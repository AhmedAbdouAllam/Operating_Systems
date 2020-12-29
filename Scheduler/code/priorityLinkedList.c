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
 
 // FOR TESTING ONLY
 void printList(struct PriorityLinkedList* pll)
 {
 	
 	struct Node* temp=pll->head;
 	if(temp==NULL)
 		printf("EMPTY \n");
 	while(temp!=NULL)
 	{
 		printf("%d  %d\n",temp->ID,temp->priorityOfList);
 		temp=temp->next;
 	
 	}
 	printf("*********************\n");
 
 }

// testing utilities
int main() 
{ 
    struct PriorityLinkedList* p = createPriorityLinkedList();
    Insert(p,1,0,0,0,5);
    Insert(p,2,0,0,0,4);
    Insert(p,3,0,0,0,20);
    Insert(p,4,0,0,0,8);
    Insert(p,5,0,0,0,7);
    printList(p);
    printf("%d  %d\n",p->head->ID,p->tail->ID);
    bool flag;
    flag=Delete(p,1);
    printList(p);
    flag=Delete(p,2);
    printList(p);
    flag=Delete(p,3);
    if(flag)	
    printf("FOUND \n");
    else
    printf("NOT FOUND \n");
    printList(p);
    flag=Delete(p,4);
    printList(p);
    flag=Delete(p,5);
    printList(p);
    flag=Delete(p,1);
    if(flag)	
    printf("FOUND \n");
    else
    printf("NOT FOUND \n");
    struct PriorityLinkedList* p2 = createPriorityLinkedList();
    Insert(p2,1,0,0,0,3);
    Insert(p2,2,0,0,0,3);
    Insert(p2,3,0,0,0,5);
    Insert(p2,4,0,0,0,8);
    Insert(p2,5,0,0,0,8);
    printList(p2);
    printf("%d  %d\n",p2->head->ID,p2->tail->ID);
   
    flag=Delete(p2,1);
    printList(p2);
    flag=Delete(p2,2);
    printList(p2);
    flag=Delete(p2,3);
    if(flag)	
    printf("FOUND \n");
    else
    printf("NOT FOUND \n");
    printList(p2);
    flag=Delete(p2,4);
    printList(p2);
    flag=Delete(p2,5);
    printList(p2);
    flag=Delete(p2,1);
    if(flag)	
    printf("FOUND \n");
    else
    printf("NOT FOUND \n");
    struct PriorityLinkedList* p3 = createPriorityLinkedList();
    Insert(p3,1,0,0,0,20);
    Insert(p3,2,0,0,0,20);
    Insert(p3,3,0,0,0,12);
    Insert(p3,4,0,0,0,11);
    Insert(p3,5,0,0,0,11);
    printList(p3);
    printf("%d  %d\n",p3->head->ID,p3->tail->ID);
    flag=Delete(p3,1);
    printList(p3);
    flag=Delete(p3,2);
    printList(p3);
    flag=Delete(p3,3);
    if(flag)	
    printf("FOUND \n");
    else
    printf("NOT FOUND \n");
    printList(p3);
    flag=Delete(p3,4);
    printList(p3);
    flag=Delete(p3,5);
    printList(p3);
    flag=Delete(p3,1);
    if(flag)	
    printf("FOUND \n");
    else
    printf("NOT FOUND \n");
    return 0; 
}
