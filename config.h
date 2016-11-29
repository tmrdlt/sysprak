
#ifndef config_h
#define config_h

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct{
    char hostname [30];
    int portnumber;
    char gamekindname[30];
}config;
/* #define  HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define  PORTNUMBER 1357
#define  GAMEKINDNAME "Bashni"
*/
void openconfig(char *filename);




#endif
