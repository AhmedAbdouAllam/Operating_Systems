#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>
#include <sys/types.h>
#include <unistd.h>



void clearResources(int signum);// sig handler
void ProcessGeneratorMainLoop();
void CreateChildreen(int QT,int c,int NumberOfProcesses);
