#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stat.h"
/*
Authors: Nathaniel Harder, Brendan Embrey
Assignment Number: 1
Date of Submission:
Name of this file: 5ps.c
Description of the program:
*/

int handle_args(int argc, char *argv[], int process_information, int state_information, int time, int memory, int command_line, char pid[]) {
    for(int index = 1; index < argc; index++) {
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
                                printf("Error: selected pid is not valid.\n");
                                exit(0);
                            } 
                        }

                        pid = argv[index];
                    break;
                }
    }
    if(strcmp(pid, "") == 0) {
        pid[0] = '1';
    }
    if(process_information == 1) {
        parse(pid, state_information, time, memory, command_line);
    } else {
        printf("You must select a PID with the option -p.\n");
        exit(0);
    }
}


int main(int argc, char *argv[]) {
int process_information = 0;
int state_information = 0;
int time = 0;
int memory = 0;
int command_line = 0;
char pid[9] = "";
handle_args(argc, argv, process_information, state_information, time, memory, command_line, pid);
return 0;
}
