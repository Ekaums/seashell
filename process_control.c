#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "process_control.h"
#include "input_parse.h"

static void path_command(char *arg);
static int builtin_command(char *arg);

void process(char *arg){

    if(builtin_command(arg)){ // Run cd or exit
        return;
    }
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

    char **args = parse_tokens(arg);
    execv(args[0], args);

    // Should never reach here
    printf("Failed command\n");
    exit(1);
}

int builtin_command(char *arg){

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
        return 1;
    }
    return 0;
}