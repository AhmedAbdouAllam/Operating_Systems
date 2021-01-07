//Author: Ahmed A.Allam
//25-12-2020
#define MSG_MAX 256
#define MEM_KeyID 65
#define S1_KeyID 64
#define S2_KeyID 63
#define S3_KeyID 62
#define S4_KeyID 61
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/sem.h>
int shmid ;
void *shmaddrx;
int main()
{

	key_t Memory = ftok("keyfile", 100);
	shmid = shmget(Memory, 4096, IPC_CREAT | 0666);
    	shmaddrx = shmat(shmid, (void *)0, 0);
while(1)
{    	
    	char SentString[10];
    	int n;
    	scanf("%d",&n);
	sprintf(SentString, "%d", n);
	
		strcpy((char *)shmaddrx, SentString);	

}



}
