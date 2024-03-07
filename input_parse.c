#include <ctype.h>
#include <string.h>
#include "input_parse.h"


char *trim_whitespace(char *string){
    if(string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0'; // Remove newline

    while(isspace(*string)) string++; // Increment start of string

    if(*string == 0) return string;

    char *end = string + strlen(string) - 1;
    while(end > string && isspace(*end)) end--; // Decrement end of string
    end[1] = '\0';

    return string;
}