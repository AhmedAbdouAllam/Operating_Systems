#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include "Process_Generator_Function.h"
#include "headers.h"
////////////////////////////////////////////////////////////////////////////////


int main(int argc, char * argv[])
{	//Ahmed A.Allam We need to Clean up this a little bit :D 
	//signal(SIGINT, clearResources);
	key_t ProcessGen_Scheduler_GenKey = ftok("keyfile", 10);
	int ProcessGen_Scheduler_Key = msgget(ProcessGen_Scheduler_GenKey, 0666 | IPC_CREAT);
	struct Process* Processes=NULL;
	int NumberOfProcesses=0;
	
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
   	int id=0,AT=0,RT=0,p=0;
   	while(read=getline(&line,&len,fp)!=-1)
   	{
   		if(line[0]!='#')
   		{
   			NumberOfProcesses+=1;
   			sscanf(line,"%d %d %d %d\n", &id,&AT,&RT,&p);
   			Processes = (struct Process*) realloc(Processes,NumberOfProcesses*sizeof(struct Process) );
   			Processes[NumberOfProcesses-1].Arrival_Time=AT;
   			Processes[NumberOfProcesses-1].Running_Time=RT;
   			Processes[NumberOfProcesses-1].Priority=p;
   			Processes[NumberOfProcesses-1].Process_ID=id;
   		}
   	}

    //Test..........
    printf("Number of Proceses %d\n",NumberOfProcesses);
	for(int i=0;i<NumberOfProcesses;i++)
	{
	
		printf("Process ID : %d\n",Processes[i].Process_ID);
	
	}      
    /////////////////////
    
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    
    	printf("Choose your scheduling algorithm: \n 1 for HPF \n 2 for SRTN \n 3 for RR \n");
    	int c=0,TQ=0;
    	scanf("%d",&c);
	if(c==3)
	{
		printf("Please enter desired time quantum\n");
		scanf("%d",&TQ);
		printf("%d",TQ);
	}
    // 3. Initiate and create the scheduler and clock processes.
	// We Start the scheduler process by forking a new process and making it run it
	int pid_CLK =fork();
	if (pid_CLK == -1)
	{
		printf("\nFailed to Fork the Clock Process\n");	
		
	}  		
  	
  	else if (pid_CLK == 0)
 	{ 
 		execl("./CLK.out", "CLK.out", NULL);
 	}
	
	int pid_SCHE =fork();
	if (pid_SCHE == -1)
	{
		printf("\nFailed to Fork the Scheduler Process\n");	
		
	}  		
  	
  	else if (pid_SCHE == 0)
 	{ 
 		char QuantamTimeString[10];
 		sprintf(QuantamTimeString, "%d", TQ); 
 		char NumberOfProcessesString[10];
 		sprintf(NumberOfProcessesString, "%d", NumberOfProcesses);
 		char SchedulaingType[10];
 		sprintf(SchedulaingType, "%d", c); 	
 		printf("%s","Create Scheduler");
 		execl("./scheduler.out", "scheduler.out",SchedulaingType,QuantamTimeString,NumberOfProcessesString, NULL);
 		exit(0);
 	}
 	
 	printf("PID SCHE %d\n",pid_SCHE);
 	
    // 4. Use this function after creating the clock process to initialize clock
	initClk();
    // To get time use this
    	int Current=getClk();
	int HeadProcess=0;
	struct msgbuff Message;
	while(true)
	{	
		if(HeadProcess<NumberOfProcesses)
		{
			while(Processes[HeadProcess].Arrival_Time!=Current)
			{
				Current= getClk();
			}
		//	kill(pid_SCHE,SIGUSR1);
			Message.Proc=Processes[HeadProcess];
			Message.mtype=1;
			msgsnd(ProcessGen_Scheduler_Key, &Message, sizeof(Message.Proc), !IPC_NOWAIT);
			printf("current time is after loop %d\n", Current);
			printf("Current Process %d\n", Processes[HeadProcess].Process_ID);
			HeadProcess+=1;

		}
	}
    // 7. Clear clock resources
	destroyClk(true);	
	}
	




