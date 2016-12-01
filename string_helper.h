//
//  string_helper.h
//  bashni
//
//  Created by Holzner on 01.12.16.
//  Copyright © 2016 System-Praktikum. All rights reserved.
//

#ifndef string_helper_h
#define string_helper_h

#include <stdio.h>
#include <stdlib.h>


int split(char *string_to_split , char delimiter , char ***dest);

char *float_to_string(float _float);

char *int_to_string(int _int);

float string_to_float(char *string);

int string_to_int(char *string);

#endif /* string_helper_h */
