
#ifndef config_h
#define config_h

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define CONFIG_ZEICHEN 50
#define CONFIG_ZEILEN 3


typedef struct{
    char gamekindname[CONFIG_ZEICHEN];
    char hostname[CONFIG_ZEICHEN];
    int portnumber;

}config;

extern config _config;

void openconfig(char *filename);

void removeSpaces(char* string);

void getmethevalue(char string[CONFIG_ZEICHEN]);


#endif
