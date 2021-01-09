/**
 * @file input_int.h
 * @brief Sammlung von Funktionen für die Eingabe von int-Werten über die Konsole
 * @version 0.1
 * @date 2020-12-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

// Include Guard setzen
#ifndef  INPUT_INT
#define INPUT_INT

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_int_input(int* save_loc);

int check_int_input(const char* input_string, const unsigned long length);

#endif
