#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
FILE *fptr;
int currentTime;
void Continue(int signum)
{
currentTime=getClk();
}
int main(int agrc, char * argv[])
{
    initClk();
signal(SIGCONT,Continue);    
    //TODO it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
   currentTime=getClk();
   
    while (remainingtime > 0)
    {
  
    if(getClk()-currentTime!=0)
	{
     		remainingtime-=(getClk()-currentTime) ;
     		currentTime=getClk();
     	}

    }
     	fptr = fopen("./scheduler.log", "a");
    			char outputString[120];
    			sprintf(outputString,"Process file say hello Rem %d Current %d\n",remainingtime,currentTime); 
    			fwrite(outputString, 1, strlen(outputString), fptr);
    			fclose(fptr);    
    destroyClk(false);
    kill(getppid(),SIGUSR1);
    return 0;
}

