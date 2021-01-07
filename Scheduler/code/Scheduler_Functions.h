	#include "headers.h"
	#include "queue.c"
	#include "PriorityQueue.c"
	#include <math.h>

	// All data used in scheduler
	enum Scheduling_Algorithm {HPF_algo = 1, SRTN_algo = 2, RR_algo = 3};
	struct Queue* Process_Queue;
	struct PCB WorkingProcess;
	struct PriorityQueue* Process_PQueue;
	int QuantamTime,SchedualingTechnique,NumberOfProcesses;
	int ProcessGen_Scheduler_Key ;
	int ProcessorBusy=0;
	int FinishedProcesses=0;
	long WaitingTotal=0;
	long RunningTotal=0;
	double WTATotal=0.0;
	double WTAarrayTotal=0.0;
	float* WTAarray=NULL;
	int CurrentClock;
	FILE *fptr;
	int shmid ;
	void *shmaddrx;
	
	/*
		return number of dicimals should be printed
	*/
	int roundingDecimal(float value)
	{
		float n=10.0;
		float flag=value-(int)value;
		if(flag==0.0 )
			return 3;
		else
		{
			flag=value*n;
		 	if((flag-(int)flag)==0.0)
				return 1;
			else 
				return 2;
		}
		
	}
	
	/*
		print outputs in scheduler.perf
	*/
	void printingPerf()
	{	
		int rounded1;
		float AvgWTA=roundf(((float)WTATotal/NumberOfProcesses)* 100.0) / 100.0;
		float AvgWait=roundf(((float)WaitingTotal/NumberOfProcesses)* 100.0) / 100.0;
		for(int i=0;i<NumberOfProcesses;i++)
		{

			WTAarrayTotal+=pow((float)(WTAarray[i]-AvgWTA),2.0);
		}
		float std=roundf((sqrt((float)WTAarrayTotal/NumberOfProcesses))*100.0)/100.0;	
		float utilization=roundf((((float)RunningTotal/getClk())*100.0)*100.0)/100.0;
		rounded1=roundingDecimal(utilization);
		fptr = fopen("./scheduler.perf", "a");
		char outputString1[120];
		if(rounded1==3)
			sprintf(outputString1,"CPU utilization = %d%%\n",(int)utilization); 			else if(rounded1==1)
			sprintf(outputString1,"CPU utilization = %.1f%%\n",(float)utilization);
		else
			sprintf(outputString1,"CPU utilization = %.2f%%\n",(float)utilization);
						
		fwrite(outputString1, 1, strlen(outputString1), fptr);
		char outputString2[120];
		rounded1=roundingDecimal(AvgWTA);
		if(rounded1==3)
			sprintf(outputString2,"Avg WTA = %d\n",(int)AvgWTA); 			else if(rounded1==1)
			sprintf(outputString2,"Avg WTA = %.1f\n",(float)AvgWTA);
		else
			sprintf(outputString2,"Avg WTA = %.2f\n",(float)AvgWTA);
		fwrite(outputString2, 1, strlen(outputString2), fptr);
		char outputString3[120];
		rounded1=roundingDecimal(AvgWait);
		if(rounded1==3)
			sprintf(outputString3,"Avg Waiting = %d\n",(int)AvgWait); 			else if(rounded1==1)
			sprintf(outputString3,"Avg Waiting = %.1f\n",(float)AvgWait);
		else
			sprintf(outputString3,"Avg Waiting = %.2f\n",(float)AvgWait); 			fwrite(outputString3, 1, strlen(outputString3), fptr);
		char outputString4[120];
		rounded1=roundingDecimal(std);
		if(rounded1==3)
			sprintf(outputString4,"Std WTA = %d\n",(int)std); 			   			else if(rounded1==1)
			sprintf(outputString4,"Std WTA = %.1f\n",(float)std);
		else
			sprintf(outputString4,"Std WTA = %.2f\n",(float)std); 			fwrite(outputString4, 1, strlen(outputString4), fptr); 				
		fclose(fptr);
	
	}
	

	/*
		Check if new Process received and enqueue in current DS
	*/
	void CheckNewProcessRecieved()
	{

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
			Added_PCB.totalTime=Added_PCB.Proc.Running_Time;
			RunningTotal+=Added_PCB.totalTime;
			Added_PCB.waitingTime=0;

			if(SchedualingTechnique==HPF_algo)
		PriorityenQueue(Process_PQueue,Added_PCB,Added_PCB.Proc.Priority);
			else if(SchedualingTechnique==SRTN_algo)	PriorityenQueue(Process_PQueue,Added_PCB,Added_PCB.Proc.Running_Time);
			else if(SchedualingTechnique==RR_algo)
			enQueue(Process_Queue,Added_PCB);
			}
		}
	}


	/*
		write process PCB in outputfile
	*/
	
	void Handle_Process_Termination(int signum)
	{	
		FinishedProcesses+=1;
		ProcessorBusy=0;
		CurrentClock=getClk();
		WorkingProcess.FinishTime=CurrentClock;
		int TA=WorkingProcess.FinishTime-WorkingProcess.Proc.Arrival_Time;
		//float WTA=(int)(((float)TA/WorkingProcess.totalTime)*100+0.5);
		//WTA=(float)WTA/100;
		WTATotal+=((float)TA/WorkingProcess.totalTime);
		WaitingTotal+=WorkingProcess.waitingTime;
		float WTA=roundf(((float)TA/WorkingProcess.totalTime)*100.0)/100.0;	
		WTAarray = (float*) realloc(WTAarray,FinishedProcesses*sizeof(float));
		WTAarray[FinishedProcesses-1]=WTA;
		int rounded=roundingDecimal(WTA);
		fptr = fopen("./scheduler.log", "a");
		char outputString[120];
		if(rounded==3)
		 	sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,0,WorkingProcess.waitingTime,TA,(int)WTA); 
		else if(rounded==1)
			sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.1f\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,0,WorkingProcess.waitingTime,TA,(float)WTA);
		else
			sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,0,WorkingProcess.waitingTime,TA,(float)WTA);
		fwrite(outputString, 1, strlen(outputString), fptr);
		fclose(fptr);
	}

	/*
		Process Termination in SRTN scheduling algorithm
	*/
	void SRTNTermination()
	{	
		for(int i=0;i<100;i++);
		kill(WorkingProcess.PID,SIGSTOP);
		ProcessorBusy=0;
		WorkingProcess.Proc.Running_Time-=(CurrentClock-WorkingProcess.LastStartTime);
		WorkingProcess.FinishTime=CurrentClock;
		
		if(WorkingProcess.Proc.Running_Time!=0)
			{	
						PriorityenQueue(Process_PQueue,WorkingProcess,WorkingProcess.Proc.Running_Time);
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				sprintf(outputString,"At time %d process %d stopped arr %d total %d remain %d wait %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
			}
			/*else
			{	
				WorkingProcess.FinishTime=CurrentClock;
				int TA=WorkingProcess.FinishTime-WorkingProcess.Proc.Arrival_Time;
				float WTA=(int)(((float)TA/WorkingProcess.totalTime)*100.0+.5);
				WTA=(float)WTA/100.0;
				int rounded=roundingDecimal(WTA);
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				if(rounded==3)
				 	sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime,TA,(int)WTA); 
				else if(rounded==1)
					sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %0.1f\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime,TA,(float)WTA);
				else
					sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %0.2f\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime,TA,(float)WTA);
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
			}*/
	}

	/*
		Logic of SRTN scheduling algorithm
	*/
	void SRTN()
	{

	while (FinishedProcesses<NumberOfProcesses)
		{
			CurrentClock=getClk();
			CheckNewProcessRecieved();
			if(ProcessorBusy==0 &&Process_PQueue->front!=NULL )	
			{
				WorkingProcess=Process_PQueue->front->Process_Control_Block;
				if(WorkingProcess.FinishTime!=0)
					WorkingProcess.waitingTime=CurrentClock-WorkingProcess.FinishTime;
				else
					WorkingProcess.waitingTime=CurrentClock-WorkingProcess.Proc.Arrival_Time;
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				if(WorkingProcess.FinishTime!=0)
					sprintf(outputString,"At time %d process %d resumed arr %d total %d remain %d wait %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 

				else
					sprintf(outputString,"At time %d process %d started arr %d total %d remain %d wait %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
				PrioritydeQueue(Process_PQueue);
				if(WorkingProcess.PID==-1)
				{
					int pid=fork();
			
					if (pid == -1)
					{}  		
	  	
	  				else if (pid == 0)
	 				{ 
		 				char RemainingTime[10];
	 					char CurrentClockStr[10];
	 					sprintf(CurrentClockStr, "%d", CurrentClock);
	 					sprintf(RemainingTime, "%d", WorkingProcess.Proc.Running_Time);
	 					char SentString[10];
						sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time);
						strcpy((char *)shmaddrx, SentString);	
						execl("./process.out", "process.out",RemainingTime, CurrentClockStr,NULL);
						exit(0);
						
				
					}	
					else
					{
						WorkingProcess.PID=pid;
						WorkingProcess.StartTime=CurrentClock;
						WorkingProcess.LastStartTime=CurrentClock;
						ProcessorBusy=1;
					}
				}
				else
				{
					WorkingProcess.LastStartTime=CurrentClock;
					char SentString[10];
					sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time-(CurrentClock-WorkingProcess.LastStartTime));
					strcpy((char *)shmaddrx, SentString);
					kill(WorkingProcess.PID,SIGCONT); 
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
		char SentString[10];
		sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time-(CurrentClock-WorkingProcess.LastStartTime));
		strcpy((char *)shmaddrx, SentString);
	}
		printingPerf();
	}

	/*
		Process Termination in Round Robin scheduling algorithm
	*/

	void RoundRobinQuantamFinished ()
	{	
		for(int i=0;i<100;i++);
		kill(WorkingProcess.PID,SIGSTOP);
		ProcessorBusy=0;
		WorkingProcess.Proc.Running_Time-=(CurrentClock-WorkingProcess.LastStartTime);//Could be done by subtracting Quantum
		WorkingProcess.FinishTime=CurrentClock;
		
		if(WorkingProcess.Proc.Running_Time!=0)
			{	
				enQueue(Process_Queue,WorkingProcess);
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				sprintf(outputString,"At time %d process %d stopped arr %d total %d remain %d wait %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
			}
			/*else
			{	
				WorkingProcess.FinishTime=CurrentClock;
				int TA=WorkingProcess.FinishTime-WorkingProcess.Proc.Arrival_Time;
				float WTA=(int)(((float)TA/WorkingProcess.totalTime)*100.0+.5);
				WTA=(float)WTA/100.0;
				int rounded=roundingDecimal(WTA);
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				if(rounded==3)
				 	sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime,TA,(int)WTA); 
				else if(rounded==1)
					sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %0.1f\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime,TA,(float)WTA);
				else
					sprintf(outputString,"At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %0.2f\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime,TA,(float)WTA);
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
			}*/
	}

	/*
		Logic of Round Robin Scheduling Algorithm
	*/
	void RoundRobin()
	{
		while (FinishedProcesses<NumberOfProcesses)
		{
			CurrentClock=getClk();
			CheckNewProcessRecieved();
			if(ProcessorBusy==0 &&Process_Queue->front!=NULL )	
			{
				WorkingProcess=Process_Queue->front->Process_Control_Block;
				if(WorkingProcess.FinishTime!=0)
					WorkingProcess.waitingTime=CurrentClock-WorkingProcess.FinishTime;
				else
					WorkingProcess.waitingTime=CurrentClock-WorkingProcess.Proc.Arrival_Time;
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				if(WorkingProcess.FinishTime!=0)
					sprintf(outputString,"At time %d process %d resumed arr %d total %d remain %d wait %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 

				else
					sprintf(outputString,"At time %d process %d started arr %d total %d remain %d wait %d\n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
					
				deQueue(Process_Queue);
				if(WorkingProcess.PID==-1)
				{
					int pid=fork();
			
					if (pid == -1)
					{}  		
					
	  				else if (pid == 0)
	 				{ 
		 				char RemainingTime[10];
	 					char CurrentClockStr[10];
	 					sprintf(CurrentClockStr, "%d", CurrentClock);
	 					sprintf(RemainingTime, "%d", WorkingProcess.Proc.Running_Time);
	 											char SentString[10];
		sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time);
		
			strcpy((char *)shmaddrx, SentString);	
						execl("./process.out", "process.out",RemainingTime,CurrentClockStr, NULL);
						exit(0);
					}	
					else
					{
						WorkingProcess.PID=pid;
						WorkingProcess.StartTime=CurrentClock;
						WorkingProcess.LastStartTime=CurrentClock;
						ProcessorBusy=1;
					}
				}
				else
				{
					WorkingProcess.LastStartTime=CurrentClock;
					char SentString[10];
					sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time-(CurrentClock-WorkingProcess.LastStartTime));
					strcpy((char *)shmaddrx, SentString);
					kill(WorkingProcess.PID,SIGCONT); 
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
		char SentString[10];
		sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time-(CurrentClock-WorkingProcess.LastStartTime));
		strcpy((char *)shmaddrx, SentString);
	}
		printingPerf();
	}

	/*
		Logic of HPF Scheduling algorithm
	*/
	void HPF()
	{
		while (FinishedProcesses<NumberOfProcesses)
		{
			CurrentClock=getClk();
			CheckNewProcessRecieved();
			if(ProcessorBusy==0 &&Process_PQueue->front!=NULL )	
			{
				WorkingProcess=Process_PQueue->front->Process_Control_Block;
				WorkingProcess.waitingTime=CurrentClock-WorkingProcess.Proc.Arrival_Time;
				fptr = fopen("./scheduler.log", "a");
				char outputString[120];
				sprintf(outputString,"At time %d process %d started arr %d total %d remain %d wait %d \n",CurrentClock,WorkingProcess.Proc.Process_ID,WorkingProcess.Proc.Arrival_Time,WorkingProcess.totalTime,WorkingProcess.Proc.Running_Time,WorkingProcess.waitingTime); 
				fwrite(outputString, 1, strlen(outputString), fptr);
				fclose(fptr);
				PrioritydeQueue(Process_PQueue);
				if(WorkingProcess.PID==-1)
				{
					int pid=fork();
			
					if (pid == -1)
					{}  		
	  	
	  				else if (pid == 0)
	 				{ 
		 				char RemainingTime[10];
	 					char CurrentClockStr[10];
	 					sprintf(CurrentClockStr, "%d", CurrentClock);
	 					sprintf(RemainingTime, "%d", WorkingProcess.Proc.Running_Time);
	 					char SentString[10];
						sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time);
						strcpy((char *)shmaddrx, SentString);	
						execl("./process.out", "process.out",RemainingTime,CurrentClockStr,NULL);
						exit(0);
				
					}	
					else
					{
						WorkingProcess.PID=pid;
						WorkingProcess.StartTime=CurrentClock;
						WorkingProcess.LastStartTime=CurrentClock;
						ProcessorBusy=1;
					}
				}
				else
				{
					WorkingProcess.LastStartTime=CurrentClock;
					char SentString[10];
					sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time-(CurrentClock-WorkingProcess.LastStartTime));
					strcpy((char *)shmaddrx, SentString);	
					kill(WorkingProcess.PID,SIGCONT); 
					ProcessorBusy=1;
				}		
		}
		char SentString[10];
		sprintf(SentString, "%d", WorkingProcess.Proc.Running_Time-(CurrentClock-WorkingProcess.LastStartTime));
		strcpy((char *)shmaddrx, SentString);
	}
		printingPerf();
	}

	
