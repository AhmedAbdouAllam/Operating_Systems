#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>
//////////////////////////////////////////////////////////////////////////////////
struct node
{
	int id;
	int prioprity;
	int arriavalTime;
	int runningTime;
	struct node *Next;
	struct node *Back;
};


////////////////////////////////////////////////////////////////////////////////
void clearResources(int);// sig handler

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    
    FILE *fp=fopen(argv[1],"r");
     if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(-1);
   }


   ssize_t read,len=0;
   char*line;
   int id=0,AT=0,WT=0,p=0;
   while(read=getline(&line,&len,fp)!=-1)
   {
   if(line[0]!='#')
   {
  sscanf(line,"%d %d %d %d\n", &id,&AT,&WT,&p);
  printf("%d %d %d %d\n", id,AT,WT,p);
   }
   }

    
    
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    
    printf("Choose your scheduling algorithm: \n 1 for HPF \n 2 for SRTN \n 3 for RR \n");
    int c=0,TQ=0;
    scanf("%d",&c);
  if(c==3)
  {printf("Please enter desired time quantum\n");
  scanf("%d",&TQ);
  printf("%d",TQ);
  }
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
   // initClk();
    // To get time use this
    //int x = getClk();
   // printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
   // destroyClk(true); //UN COMMENT
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}