
#ifndef config_h
#define config_h

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct{
    char *hostname;
    int portnumber;
    char *gamekindname;
}config;

void openconfig(char *filename);

void removeSpaces(char* string);

void getmethevalue(char ** string);


#endif
