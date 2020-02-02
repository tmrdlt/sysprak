#ifndef config_h
#define config_h

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <errno.h>

#define STANDARD_HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define STANDARD_GAMEKINDNAME "Bashni"
#define STANDARD_PORTNUMBER 1357
// zum erweitern um weitere parameter den Wert ZEILEN erhöhen
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
