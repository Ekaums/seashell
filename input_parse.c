#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "input_parse.h"

char **parse_tokens(char *string){
    
    size_t capacity = 2; // Starts at 2 to have arg + NULL (NULL added later)
    char **args = (char**)malloc(sizeof(char *) * capacity); // Array of strings for execv
    if(!args) assert(args);

    // Check if access /bin/token, then /usr/bin/token
    char *token = strsep(&string, " ");
    char *test = strdup("/bin/");
    strcat(test, token);

    if(access(test, X_OK) == 0){
        args[0] = strdup(test);
    }
    else{
        test = strdup("/usr/bin/");
        strcat(test, token);

        if(access(test, X_OK) == 0){
            args[0] = strdup(test);
        }
        else{
            printf("Invalid Command\n");
            exit(1);
        }
    }


    size_t index = 1; // Index for next element (excluding NULL)
    while((token = strsep(&string, " "))){
        // Double size of array if more args
        if (index+1 >= capacity){ // Index is 0-indexed and capacity is 1-indexed, so need +1
            capacity *= 2; 
            args = realloc(args, capacity * sizeof(char *));
            if(!args) assert(args);
        }
        
        args[index++] = token;
     }

    args[index] = NULL; // Add NULL before calling execv
    return args;
}





char *trim_whitespace(char *string){
    if(string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0'; // Remove newline

    while(isspace(*string)) string++; // Increment start of string

    if(*string == 0) return string;

    char *end = string + strlen(string) - 1;
    while(end > string && isspace(*end)) end--; // Decrement end of string
    end[1] = '\0';

    return string;
}