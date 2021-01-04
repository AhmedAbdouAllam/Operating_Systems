#include "headers.h"
#include "queue.c"
#include "PriorityQueue.c"
// Required to clean up this file using Scheduler_Functions.h ////Ahmed A.Allam....
//#include "Scheduler_Functions.h"
struct Queue* Process_Queue;
struct PCB WorkingProcess;
struct PriorityQueue* Process_PQueue;
int QuantamTime,SchedualingTechnique,NumberOfProcesses;
int ProcessGen_Scheduler_Key ;
int ProcessorBusy=0;
int FinishedProcesses=0;
int CurrentClock;
int z;
bool WorkingProcessFlag=false;
FILE *fptr;

void Handle_Process_Termination(int signum)
{	
	//alarm(0);
	FinishedProcesses+=1;
	ProcessorBusy=0;
	CurrentClock=getClk();
		//printf("At time %d process %d Finished arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
	fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d finished arr %d remain %d terminate\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
}

void RoundRobinQuantamFinished ()
{	
	kill(WorkingProcess.PID,SIGSTOP);
	ProcessorBusy=0;
//here we may write to files.....
	WorkingProcess.Proc.Running_Time-=(CurrentClock-WorkingProcess.LastStartTime); //Could be done by subtracting Quantum
	if(WorkingProcess.Proc.Running_Time!=0)
		{
			enQueue(Process_Queue,WorkingProcess);
		//	printf("At time %d process %d stopped arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
			fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d stopped arr %d remain %d\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
		
		
		}
		else
		{
		//printf("At time %d process %d Finished arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
			fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d finished arr %d remain %d prediction\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
		
		}
}

void SRTNTermination()
{
kill(WorkingProcess.PID,SIGSTOP);
	ProcessorBusy=0;
//here we may write to files.....
	WorkingProcess.Proc.Running_Time-=(CurrentClock-WorkingProcess.LastStartTime); //Could be done by subtracting Quantum
	if(WorkingProcess.Proc.Running_Time!=0)
		{
			PriorityenQueue(Process_PQueue,WorkingProcess,WorkingProcess.Proc.Running_Time);
			
		//	printf("At time %d process %d stopped arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
			fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d stopped arr %d remain %d\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
		
		
		}
		else
		{
		//printf("At time %d process %d Finished arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
			fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d finished arr %d remain %d prediction\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
		
		}
}
void CheckNewProcessRecieved()
{

//	//printf("\nreciption\n");
	struct msgbuff RecievedMessage;
	int RecieveSuccess=0;
	while(RecieveSuccess!=-1)
	{
	RecieveSuccess= msgrcv(ProcessGen_Scheduler_Key, &RecievedMessage, sizeof(RecievedMessage.Proc), 0, IPC_NOWAIT);
		if(RecieveSuccess!=-1)
		{
		struct PCB Added_PCB;
		Added_PCB.Proc=RecievedMessage.Proc;
		Added_PCB.StartTime=-1;
		Added_PCB.FinishTime-1;
		Added_PCB.PID=-1;
	

		if(SchedualingTechnique==1)
	PriorityenQueue(Process_PQueue,Added_PCB,Added_PCB.Proc.Priority);
		else if(SchedualingTechnique==2)	PriorityenQueue(Process_PQueue,Added_PCB,Added_PCB.Proc.Running_Time);
		else if(SchedualingTechnique==3)
		enQueue(Process_Queue,Added_PCB);
		}
	}
}

void RoundRobin()
{
	while (FinishedProcesses<NumberOfProcesses)
	{
		CurrentClock=getClk();
		CheckNewProcessRecieved();
		if(ProcessorBusy==0 &&Process_Queue->front!=NULL )	
		{
			WorkingProcess=Process_Queue->front->Process_Control_Block;
			//printf("At time %d process %d started arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
				fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d started arr %d remain %d\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
			deQueue(Process_Queue);
			if(WorkingProcess.PID==-1)
			{
				int pid=fork();
		
				if (pid == -1)
				{
					//printf("\nFailed to Fork a Process\n");			
			
				}  		
  	
  				else if (pid == 0)
 				{ 
 					char RemainingTime[10];
 					sprintf(RemainingTime, "%d", WorkingProcess.Proc.Running_Time);
					execl("./process.out", "process.out",RemainingTime, NULL);
					exit(0);
			
				}	
				else
				{
					WorkingProcess.PID=pid;
					WorkingProcess.StartTime=CurrentClock;
					WorkingProcess.LastStartTime=CurrentClock;
					//alarm(QuantamTime);
					ProcessorBusy=1;
				}
					
			
		
			}
			else
			{
				WorkingProcess.LastStartTime=CurrentClock;
				kill(WorkingProcess.PID,SIGCONT); 
				//alarm(QuantamTime);
				ProcessorBusy=1;
			}	
		
	}
	else if (ProcessorBusy==1)
	{
		if(CurrentClock-WorkingProcess.LastStartTime>=QuantamTime)
		{
		
		RoundRobinQuantamFinished();
		
		
		}
	
	
	}
}


}

void SRTN()
{

while (FinishedProcesses<NumberOfProcesses)
	{
		CurrentClock=getClk();
		CheckNewProcessRecieved();
		if(ProcessorBusy==0 &&Process_PQueue->front!=NULL )	
		{
			WorkingProcess=Process_PQueue->front->Process_Control_Block;
			//printf("At time %d process %d started arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
				fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d started arr %d remain %d\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
			PrioritydeQueue(Process_PQueue);
			if(WorkingProcess.PID==-1)
			{
				int pid=fork();
		
				if (pid == -1)
				{
					//printf("\nFailed to Fork a Process\n");			
			
				}  		
  	
  				else if (pid == 0)
 				{ 
 					char RemainingTime[10];
 					sprintf(RemainingTime, "%d", WorkingProcess.Proc.Running_Time);
					execl("./process.out", "process.out",RemainingTime, NULL);
					exit(0);
			
				}	
				else
				{
					WorkingProcess.PID=pid;
					WorkingProcess.StartTime=CurrentClock;
					WorkingProcess.LastStartTime=CurrentClock;
					//alarm(QuantamTime);
					ProcessorBusy=1;
				}
					
			
		
			}
			else
			{
				WorkingProcess.LastStartTime=CurrentClock;
				kill(WorkingProcess.PID,SIGCONT); 
				//alarm(QuantamTime);
				ProcessorBusy=1;
			}	
		
	}
	else if (ProcessorBusy==1)
	{
		 if(Process_PQueue->front!=NULL && Process_PQueue->front->priorityQ < WorkingProcess.Proc.Running_Time)
		{
						
		SRTNTermination();
		}
	
	}
}



}


void HPF()
{

while (FinishedProcesses<NumberOfProcesses)
	{
		CurrentClock=getClk();
		CheckNewProcessRecieved();
		if(ProcessorBusy==0 &&Process_PQueue->front!=NULL )	
		{
			WorkingProcess=Process_PQueue->front->Process_Control_Block;
			//printf("At time %d process %d started arr %d remain %d ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time);
				fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"At time %d process %d started arr %d remain %d\n ",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.Proc.Running_Time); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);
			PrioritydeQueue(Process_PQueue);
			if(WorkingProcess.PID==-1)
			{
				int pid=fork();
		
				if (pid == -1)
				{
					//printf("\nFailed to Fork a Process\n");			
			
				}  		
  	
  				else if (pid == 0)
 				{ 
 					char RemainingTime[10];
 					sprintf(RemainingTime, "%d", WorkingProcess.Proc.Running_Time);
					execl("./process.out", "process.out",RemainingTime, NULL);
					exit(0);
			
				}	
				else
				{
					WorkingProcess.PID=pid;
					WorkingProcess.StartTime=CurrentClock;
					WorkingProcess.LastStartTime=CurrentClock;
					//alarm(QuantamTime);
					ProcessorBusy=1;
				}
					
			
		
			}
			else
			{
				WorkingProcess.LastStartTime=CurrentClock;
				kill(WorkingProcess.PID,SIGCONT); 
				//alarm(QuantamTime);
				ProcessorBusy=1;
			}	
		
	}
	
}



}

int main(int argc, char * argv[])
{
    fptr = fopen("./scheduler.log", "a");
    char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
    fwrite(outputString, 1, strlen(outputString), fptr);
    fclose(fptr);
	//Signal Handlers....
	//signal(SIGALRM,RoundRobinQuantamFinished);
	signal(SIGUSR1,Handle_Process_Termination);
	////
	//printf("whoooooooooo");
	key_t ProcessGen_Scheduler_GenKey = ftok("keyfile", 10);
	ProcessGen_Scheduler_Key = msgget(ProcessGen_Scheduler_GenKey, 0666 | IPC_CREAT);	
    	
    	initClk();
    	
 	
 	SchedualingTechnique=atoi(argv[1]);
 	NumberOfProcesses=atoi(argv[3]);   
	//signal(SIGUSR1,NewProcessRecieved);
    	//printf("%d %d %d",QuantamTime,SchedualingTechnique,NumberOfProcesses);
    	//upon termination release the clock resources.
 // introduce here an enum with case as a clean up Ahmed A.Allam....
 	//printf("excute RoundRobin");
	if(SchedualingTechnique==1)
	{
		Process_PQueue=createPriorityQueue();
 		HPF();
 		}
 		//z=6;
 	else if(SchedualingTechnique==2)
 	{
 		Process_PQueue=createPriorityQueue();
 		SRTN();
 		//z=6;
 	
 	
 	}
 	
 	else
 		{
 			Process_Queue=createQueue(); 	
 			QuantamTime=atoi(argv[2]);
 			RoundRobin();
 			
 		}
 	
    	destroyClk(true);
}




/////////////////This will be cleaned up when we finish//////////////////////



