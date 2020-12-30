#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include "headers.h"
#include "queue_DS.c"



struct msgbuff ReceiveProcess( struct msgbuff temp);
struct algorithm_buffer  ReceiveParameters(struct algorithm_buffer temp);
void RoundRobin(struct Queue RR_ReadyQ, struct PCB *runningPTR, int TimeQuantum);
