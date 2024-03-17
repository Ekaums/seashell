#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "input_parse.h"

char **parse_tokens(char *string){
    size_t capacity = 8; // Argument array starts at size 8
    char **args = (char**) malloc(sizeof(char *) * capacity); // Array of strings for execv
    if(args == NULL){printf("malloc error occured"); exit(1);}

    // Check if access /bin/token, then /usr/bin/token
    char *token = strsep(&string, " "); // Get first token
    char *path_name = strdup("/bin/");
    strcat(path_name, token); // Check token with path

    if(access(path_name, X_OK) == 0){ // If successful
        args[0] = strdup(path_name);
    }
    else{
        path_name = strdup("/usr/bin/"); // Check usr path
        strcat(path_name, token);

        if(access(path_name, X_OK) == 0){
            args[0] = strdup(path_name);
        }
        else{
            printf("Invalid Command\n");
            free(args);
            free(path_name);
            exit(1);
        }
    }
    free(path_name);

    size_t index = 1; // Index for next element to insert into exec array
    while((token = strsep(&string, " ")) != NULL){
        if (token[0] == '\0') continue; // Skip empty tokens

        // Double size of array if more args
        if (index > capacity-1){
            capacity *= 2; 
            args = realloc(args, capacity * sizeof(char *));
            if(args == NULL){
                printf("realloc error occured"); 
                exit(1);
            }
        }

        if(!strcmp(token, ">")){
            char *filename = strsep(&string, " ");
            if(filename == NULL){
                printf("Invalid command");
                free(args);
                exit(1);
            }

            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open file for writing, clearing any existing text. Create it if it does not exist. Specify rwx perms

            if(fd == -1){
                printf("Error opening file");
                free(args);
                exit(1);
            }

            if(dup2(fd, STDOUT_FILENO) == -1){
                perror("dup2 error");
                close(fd);
                free(args);
                exit(1);
            }
            continue;
        }
        if(!strcmp(token, "<")){printf("redirect found\n");}

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