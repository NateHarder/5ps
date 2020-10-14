#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fps.h"
/*
Authors: Nathaniel Harder, Brendan Embrey
Assignment Number: 1
Date of Submission:
Name of this file: 5ps.c
Description of the program:
*/

int handle_args(int argc, char *argv[], int process_information, int state_information, int time, int memory, int command_line, char *pid) {
    for(int index = 2; index < 8; index++) {
                switch (argv[index][1]) {
                    case 'p':
                        process_information = 1;
                        break;
                    case 's':
                        state_information = 1;
                        break;
                    case 't':
                        time = 1;
                        break;
                    case 'c':
                        command_line = 1;
                        break;
                    case 'v':
                        memory = 1;
                        break;
                    default:
                        for(int pid_index = 0; pid_index < strlen(argv[index]); pid_index++) {
                            if(!isdigit(argv[index][pid_index])) {
                                printf("Error: selected pid is not valid.");
                                exit(0);
                            } 
                        }
                        pid = argv[index];
                    break;
                }
    }
}


int main(int argc, char *argv[]) {
int process_information = 0;
int state_information = 0;
int time = 0;
int memory = 0;
int command_line = 0;
char *pid = "1";
if(argc > 2) {
    handle_args(argc, argv, process_information, state_information, time, memory, command_line, pid);
    parse(process_information, pid, state_information, time, memory, command_line);
} else {
    printf("Error: A pid value must be selected.");
    exit(0);
    }
}
