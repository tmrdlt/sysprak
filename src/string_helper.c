//
//  string_helper.c
//  bashni
//
//  Created by Holzner on 01.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#include "string_helper.h"


void float_to_string(float _float,  char*dest){
    char str[15];
    sprintf(str, "%fl", _float);
    strcpy(dest, str);
}


void int_to_string(int _int,  char*dest){
    char str[15];
    sprintf(str, "%d", _int);
    strcpy(dest, str);
}

float string_to_float(char *string){
    return atof(string);
}


int string_to_int(char *string){
    return atoi(string);
}

//berechne Elemente vor einem Split
int count_elements(char *string_to_split , char delimiter){
    int count = 1;
    char *copy_of_string;
    copy_of_string = string_to_split;
    
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string == delimiter)
            count++;
        copy_of_string++;
    }
    
    return count;
}

//init each word array
void init_words(char *string_to_split , char delimiter,  char **dest[6]){
    
    char *copy_of_string;
    copy_of_string = string_to_split;
    int token_len = 1;
    int i = 0;
    
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string == delimiter)
        {
            char word[token_len];
            *dest[i] = word;
            token_len = 0;
            i++;
        }
        copy_of_string++;
        token_len++;
    }
    char word[token_len];
    *dest[i] = word;

}


/**
 * Zerteilt string in string array
 */
int split (char *string_to_spilt, char delimiter, char ***dest)
{
    int count = count_elements(string_to_spilt, delimiter);
    int token_len = 1;
    int i = 0;
    char *copy_of_string;
    char *tmp_dest;
    
    
    *dest = (char**) malloc(sizeof(char*) * count);
    if (*dest == NULL){
        perror("Memory allocation failed");
        exit(1);
    }
    
    // count tokens of each word and allocate each string
    copy_of_string = string_to_spilt;
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string == delimiter)
        {
            (*dest)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*dest)[i] == NULL){
                perror("Memory allocation failed");
                exit(1);
            }
            
            token_len = 0;
            i++;
        }
        copy_of_string++;
        token_len++;
    }
    (*dest)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*dest)[i] == NULL){
        perror("Memory allocation failed");
        exit(1);
    }
    
    // fill array with splited strings(words)
    i = 0;
    copy_of_string = string_to_spilt;
    tmp_dest = ((*dest)[i]);
    while (*copy_of_string != '\0')
    {
        if (*copy_of_string != delimiter && *copy_of_string != '\0')
        {
            *tmp_dest = *copy_of_string;
            tmp_dest++;
        }
        else
        {
            *tmp_dest = '\0';
            i++;
            tmp_dest = ((*dest)[i]);
        }
        copy_of_string++;
    }
    
    return count;
}
