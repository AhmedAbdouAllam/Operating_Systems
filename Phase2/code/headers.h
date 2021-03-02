	#pragma once
	#include <stdio.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/file.h>
	#include <sys/ipc.h>
	#include <sys/shm.h>
	#include <sys/sem.h>
	#include <sys/msg.h>
	#include <sys/wait.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <signal.h>

	typedef short bool;
	#define true 1
	#define false 0

	#define SHKEY 300
	
	////////////////////// MEMORY ////////////////////////////
	#define total_memory_size 1024
	
	//////////////////////////////////////////////////////////////
	struct Process
	{
		int Arrival_Time;
		int Running_Time;
		int Priority;
		int Process_ID;
		int mem_Size; //mem size of process itself

	};

	struct PCB
	{
		struct Process Proc;
		int StartTime;
		int FinishTime;
		int PID; //fork
		int LastStartTime;
		int totalTime;
		int waitingTime;
		
		int mem_start;
		int mem_end;
		
	}PCB;

	struct msgbuff
	{
		long mtype;
		struct Process Proc;
	};
	///==============================
	//don't mess with this variable//
	int * shmaddr;                 //
	//===============================

struct memNode{	 // partitions
	int size; // 2^^n
    int startLoc ;
    int endLoc;
	int pid;  //TO BE DONE // process to be allocated id 
    int allocated; // 1 --> allocated
					// 0--> not allocated
struct memNode *parent; 
	//struct memNode* sibilings[(total_memory_size/size)-1];
	//struct memNode* children[(2*total_memory_size/size)];

	
};
	

	int getClk()
	{
		return *shmaddr;
	}


	/*
	 * All process call this function at the beginning to establish communication between them and the clock module.
	 * Again, remember that the clock is only emulation!
	*/
	void initClk()
	{
		int shmid = shmget(SHKEY, 4, 0444);
		while ((int)shmid == -1)
		{
		    //Make sure that the clock exists
		    printf("Wait! The clock not initialized yet!\n");
		    sleep(1);
		    shmid = shmget(SHKEY, 4, 0444);
		}
		shmaddr = (int *) shmat(shmid, (void *)0, 0);
	}


	/*
	 * All process call this function at the end to release the communication
	 * resources between them and the clock module.
	 * Again, Remember that the clock is only emulation!
	 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
	 *                      It terminates the whole system and releases resources.
	*/

	void destroyClk(bool terminateAll)
	{
		shmdt(shmaddr);
		if (terminateAll)
		{
		    killpg(getpgrp(), SIGINT);
		}
	}
