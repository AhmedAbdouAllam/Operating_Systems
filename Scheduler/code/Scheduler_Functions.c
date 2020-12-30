#pragma once
#include "Scheduler_Functions.h"

struct msgbuff ReceiveProcess(struct msgbuff temp)
{

key_t key_id; 

    int msgq_id;
    int rec_val;

    key_id = ftok("keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);
      if (msgq_id == -1)
    {
        perror("Error in creating up queue");
        exit(-1);
    }
    rec_val = msgrcv(msgq_id, &temp, sizeof(temp.p), 0, !IPC_NOWAIT);
return temp;
}



////////////////////////////////////////////////////////////////////////////////////////
struct algorithm_buffer ReceiveParameters(struct algorithm_buffer temp)
{

key_t key_id; 

    int msgq_id;
    int rec_val;

    key_id = ftok("keyfile", 67);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);
      if (msgq_id == -1)
    {
        perror("Error in creating up queue");
        exit(-1);
    }
    rec_val = msgrcv(msgq_id, &temp, sizeof(temp.s), 0, !IPC_NOWAIT);
return temp;
}
//////////////////////////////////////////////////////////////////////////////////////

//void RoundRobin(struct Queue RR_ReadyQ, struct PCB *runningPTR,int TimeQuantum)
//{


//}




