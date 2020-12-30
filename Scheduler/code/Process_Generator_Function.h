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





void sendProcess( struct msgbuff temp);
void sendParameters(struct algorithm_buffer temp);
void clearResources(int signum);// sig handler

