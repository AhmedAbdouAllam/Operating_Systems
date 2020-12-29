#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/msg.h>
#include <string.h>
#define SETVAL 16


int shmid;         //shared memory size
int BufferSize;      //user size for the buffer

int *CircularQueue;
int SemLock;


int * IndexOfWriting;
int ProdQ; //PorducerQueue
int ConsQ; //ConsumerQueue

typedef struct msgbuff
{
    long mtype;
    int InputValue;
}msgbuff;

union Semun 
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    unsigned short *array; /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};

union Semun semun;

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}
/////////////////Initiate the Semaphore  and the Shared Memories required//////////////////////////////////

void intiate()
{
    // Create Only One Semaphore , Binary Semaphore
    SemLock = semget(7, 1, 0666 | IPC_CREAT);
    
    if (SemLock == -1 )
    {
        perror("Error in creating Semaphore");
        exit(-1);
    }

    //First Value of the semaphore should be one to allow one of the prod. or the consumer to operate
    semun.val = 1;
    if (semctl(SemLock, 0, SETVAL, semun) == -1)
    {
        perror("Error in mutex semctl");
    }
    //creates a shared memory segment
    // ID " 5"  for the Circular Queue 
    shmid = shmget(5, BufferSize, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("Consumer Buffer Shared Memory failed to allocate due to:");
        exit(1);
    }
    else
    {
        printf("Consumer Buffer Shared Memory allocated successfully. \n");
    }

    CircularQueue = (int *)shmat(shmid, 0, 0);
    if (CircularQueue == (int *)-1)
    {
        perror("Consumer Buffer failed to attach");
    }
    
  //This initiates shared memory that holds the number of elements
  //ID " 4 " for the index of writing
    int GetIndexWrite = shmget(4, sizeof(int), IPC_CREAT | 0666);
    if (GetIndexWrite < 0)
    {
        perror("Consumer Count Shared Memory failed to allocate due to:");
        exit(1);
    }
    else
    {
        printf("Consumer Count Shared Memory allocated successfully. \n");
    }

    IndexOfWriting = (int *)shmat(GetIndexWrite, 0, 0);
    if (IndexOfWriting == (int *)-1)
    {
        perror("Consumer Count failed to attach");
    }
  

}
/////////////////////////////////////////

void consumer()
{
        // Necessary for implementing the circular queue 
        int counter=0;
    while(1)
    {
        down(SemLock);
        if( IndexOfWriting[0] == (BufferSize/sizeof(int)) ) 
        //Full Buffer Case :   Give up control , Sleep and Wait for a Waking up message from the consumer
       {
            int item = CircularQueue[counter % (BufferSize/sizeof(int)) ]+1;
            printf("Item taken is  %d at location %ld \n", item,(counter % (BufferSize/sizeof(int))) );
            IndexOfWriting[0]--;counter++;
            up(SemLock);
             msgbuff result;
            result.mtype = 80;  
            int SendingFromConsumer = msgsnd(ConsQ, &result, sizeof(result.InputValue), IPC_NOWAIT);
        }      
        else if(IndexOfWriting[0] == 0) 
        // Empty Buffer Case: Generate a new item , wake the consumer and give control to allow consumption
        {
            up(SemLock);
           printf("Consumer is Sleeping, Gave up semaphore and Waiting for wake up from Producer  from channel 80  on the Producer Queue\n");
            msgbuff message;
        int ReceivingFromProd = msgrcv(ProdQ, &message, sizeof(message.InputValue), 80, !IPC_NOWAIT); //block and listen on the producer Queue for a wake up message
        }
        else  if( IndexOfWriting[0] < (BufferSize/sizeof(int)) ) 
        // Base Case: just consume an item and give up control

        {
            int item = CircularQueue[counter % (BufferSize/sizeof(int)) ]+1;
            printf("Item taken is  %d at location %ld \n", item,(counter % (BufferSize/sizeof(int))) );
            IndexOfWriting[0]--;counter++;
            up(SemLock);
        }
        sleep(1);
    }
}
///////////////////////////////////////////////////////////
int main()
{

//Generate Both Queues    
    ConsQ = msgget(69, IPC_CREAT | 0666); 
    ProdQ = msgget(67, IPC_CREAT | 0666); 
// Receive the size of the circular queue from the producer 
    msgbuff message; 
    message.InputValue = 0;
// First Message received from producer to Consumer including circular Queue on channel 7

    int size = msgrcv(ProdQ,&message,sizeof(message.InputValue),7,!IPC_NOWAIT);
    if(size == -1) 
     { perror("Failed to recieve size from producer due to ");
        exit(1);
     }
    BufferSize = message.InputValue;
    intiate();     
    consumer();
    return 0;
}



