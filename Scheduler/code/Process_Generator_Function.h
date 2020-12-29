#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>
struct Process
{
	int Arrival_Time;
	int Running_Time;
	int Priority;
	int Process_ID;

};


void clearResources(int signum);// sig handler

