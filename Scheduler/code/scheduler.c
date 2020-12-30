
#include "Scheduler_Functions.h"
#include "Scheduler_Functions.c"


int main()
{
    //initClk();


         
    struct algorithm_buffer ALGO,temp;
ALGO=ReceiveParameters(temp);

struct msgbuff received_processes,temp2; 

     if(ALGO.s.sched_no==1)
     {
     //HPF
     //printf("HPF\n");
    
     }
     else if (ALGO.s.sched_no==2)
     {
     //SRTN
      printf("SRTN\n");
     }
     else if (ALGO.s.sched_no==3) {
       struct Queue *RR_ReadyQ=createQueue();
       struct PCB* runningPTR;
     //RR 
     //take Time!
     //printf("RR with TQ = %d \n",ALGO.s.TimeQ)
     while(true){
     
  received_processes= ReceiveProcess(temp2);
  struct PCB processcontrolblock;
  
  processcontrolblock.process=received_processes.p;
  processcontrolblock.Remaining_Time=-1;
  processcontrolblock.Waiting_Time=-1;
  processcontrolblock.state=-1;
  enQueue(RR_ReadyQ, processcontrolblock); 
  //fill queue
// RoundRobin(RR_ReadyQ,runningPTR,ALGO.s.TimeQ); /// TO BE IMPLEMENTED 
  
  printf("ay haga  = %d \n",received_processes.p.Process_ID);
     }
     }
     else
      printf("HIII\n");
    
    
    
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    
 //   destroyClk(true);
}



