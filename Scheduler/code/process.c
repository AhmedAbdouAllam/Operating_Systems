	#include "headers.h"

	int remainingtime;
	FILE *fptr;
	int currentTime;
	int shmid ;
	void *shmaddrx;

	int main(int agrc, char * argv[])
	{
		key_t Memory = ftok("keyfile", 100);
		shmid = shmget(Memory, 4096, IPC_CREAT | 0666);
		shmaddrx = shmat(shmid, (void *)0, 0);
	   	remainingtime =atoi(argv[1]);
	  	currentTime=atoi(argv[2]);
		/*fptr = fopen("./scheduler.log", "a");//
		char outputString[120];//
		sprintf(outputString,"Process file say hello Rem %d Current %d\n",remainingtime,currentTime); //
		fwrite(outputString, 1, strlen(outputString), fptr);//
		fclose(fptr); //*/
		while (remainingtime > 0)
		{
		  	char recivedString [10];
			strcpy(recivedString,(char *)shmaddrx);
		  	remainingtime=atoi(recivedString);	 
		}
		
		kill(getppid(),SIGUSR1);
		return 0;
	}
