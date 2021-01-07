	#include "Scheduler_Functions.h"

	int main(int argc, char * argv[])
	{	
		key_t Memory = ftok("keyfile", 100);
		shmid = shmget(Memory, 4096, IPC_CREAT | 0666);
    	shmaddrx = shmat(shmid, (void *)0, 0);
    	/*
		fptr = fopen("./scheduler.log", "a");
		char outputString[] = "#At time x process y state arr w total z remain y wait k\n";
		fwrite(outputString, 1, strlen(outputString), fptr);
		fclose(fptr);
		*/
		signal(SIGUSR1,Handle_Process_Termination);
		key_t ProcessGen_Scheduler_GenKey = ftok("keyfile", 10);
		ProcessGen_Scheduler_Key = msgget(ProcessGen_Scheduler_GenKey, 0666 | IPC_CREAT);	
			
		initClk();
	 	SchedualingTechnique=atoi(argv[1]);
	 	NumberOfProcesses=atoi(argv[3]);
	 	   
		if(SchedualingTechnique==HPF_algo)
		{
			Process_PQueue=createPriorityQueue();
			//printf("Excute HPF\n");
	 		HPF();
	 	}
	 	else if(SchedualingTechnique==SRTN_algo)
	 	{
	 		Process_PQueue=createPriorityQueue();
	 		//printf("Excute SRTN\n");
	 		SRTN();
	 	}
	 	else
	 	{
	 		Process_Queue=createQueue();
	 		//printf("Excute RoundRobin\n"); 	
	 		QuantamTime=atoi(argv[2]);
			RoundRobin();
	 			
	 	}
	 	
			destroyClk(true);
	}

