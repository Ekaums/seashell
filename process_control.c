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

    char *temp_arg = strdup(arg); // Use temp ptr to validate argument (as strsep manipulates the pointer)
    char *next_token = strsep(&temp_arg, " ");

    if(!strcmp(next_token, "cd")){
        next_token = strsep(&temp_arg, " ");
        if(chdir(next_token) != 0){ // Change directory (if valid)
            printf("Error: Invalid directory\n");
        }
        return;
    }

    // TODO: Forking in another function
    int rc = fork();

    if (rc < 0){
        printf("fork failed\n");
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
    size_t index = 1;
    size_t capacity = 2; // Starts at 2 to have arg + NULL

    char **args = (char**)malloc(sizeof(char *) * capacity); // Cannot use char *args[]. This defines a static-sized array which cannot be malloced
    if(!args) assert(args);

    char *token = strsep(&arg, " ");

    // Check if access /bin/token, then /usr/bin/token
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

    // Parse arguments, adding them to args array
    while((token = strsep(&arg, " "))){
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