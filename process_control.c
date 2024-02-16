#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "process_control.h"

static void path_command(char *arg);

void process(char *arg){
    
    if(!strcmp(arg, "exit")){
        exit(0);
    }

    char *temp_arg = strdup(arg); // Preserve arg if needed for path command
    char *token = strsep(&temp_arg, " ");

    if(!strcmp(token, "cd")){
        token = strsep(&temp_arg, " ");
        if(chdir(token) != 0){
            printf("Error: Invalid directory\n");
        }
        return;
    }
    else if(!strcmp(token, "path")){
        
    }

    // TODO: Forking in another function
    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc == 0){ // Child
        path_command(arg);
    }
    else{ // Parent
        int rc_wait = wait(NULL);
    }

    return;
}

// Execute a path command
static void path_command(char *arg){
    size_t index = 0;
    size_t capacity = 2; // Starts at 2 to have arg + NULL

    char **args = (char**)malloc(sizeof(char *) * capacity); // Cannot use char *args[]. This defines a static-sized array which cannot be malloced
    if(!args) assert(args);

    char *token;
    // Parse arguments, adding them to args array
    while((token = strsep(&arg, " "))){
        // Ensure first command is valid
        if(index == 0){
            args[index] = strdup("/bin/"); 
            strcat(args[index], token);

            if(access(args[0], X_OK) != 0){ 
                printf("Invalid Command\n");
                exit(1);
            }
            index += 1;
            continue;
        }
        // Double size of array if more args
        if (index+1 >= capacity){
            capacity *= 2; 
            args = realloc(args, capacity * sizeof(char *));
            if(!args) assert(args);
        }
        args[index++] = token;
    }

    args[index] = NULL; // Add NULL before calling execv
    execv(args[0], args);

    // Should never reach here
    printf("Failed path command\n");
    exit(1);
}