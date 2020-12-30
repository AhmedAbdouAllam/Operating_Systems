#include "Process_Generator_Function.h"
void sendProcess( struct msgbuff temp)
{

	 key_t key_id; 
    int msgq_id, send_val;
        key_id = ftok("keyfile", 65);
        
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);
      if (msgq_id == -1)
    {
        perror("Error in creating up queue");
        exit(-1);
    }

    send_val = msgsnd(msgq_id, &temp, sizeof(temp.p), !IPC_NOWAIT);

	
}



void sendParameters(struct algorithm_buffer temp)
{

	 key_t key_id; 
    int msgq_id, send_val;
        key_id = ftok("keyfile", 67);
        
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);
      if (msgq_id == -1)
    {
        perror("Error in creating up queue");
        exit(-1);
    }

    send_val = msgsnd(msgq_id, &temp, sizeof(temp.s), !IPC_NOWAIT);

}
