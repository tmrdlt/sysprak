
#ifndef config_h
#define config_h

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct{
    char *gamekindname;
    char *hostname;
    int portnumber;
   
}config;

extern config _config;

void openconfig(char *filename);

void removeSpaces(char* string);

void getmethevalue(char ** string);


#endif
