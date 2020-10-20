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
5ps.c takes arguments supplied from the command line and parses them before passing them to the parse function
in stat.c file to print the disired data. 
*/
/*

handle_args: takes command line options and interprets data from them before passing them to be parsed.
*/
int handle_args(int argc, char *argv[], int process_information, int state_information, int time, int memory, int command_line, char pid[]) {
    
    // loop through command line arguments and set a boolean to true if 
    // the corresponding letter is found.
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
                    // If no letters are selected, ensure that the argument is a valid pid.
                        for(int pid_index = 0; pid_index < strlen(argv[index]); pid_index++) {
                            if(!isdigit(argv[index][pid_index])) {
                                // Print an error and exit if invalid.
                                printf("Error: selected pid is not a number.\n");
                                exit(0);
                            } 
                        }
                        // If the argument is a number, set the value of pid to the arguement.
                        pid = argv[index];
                    break;
                }
    }
    // If no pid is selected as an argument, default to a pid of 1.
    if(strcmp(pid, "") == 0) {
        pid[0] = '1';
    }
    // If the argument -p is used and a pid is given, call parse to return the requested data.
    if(process_information == 1) {
        parse(pid, state_information, time, memory, command_line);
    } else {
        // If -p is not used to specify a pid, give an error and exit.
        printf("You must select a PID with the option -p.\n");
        exit(0);
    }
}



/*
main: sets boolean values to zero, initializes pid to an empty string, and passes them all to handle args.
*/
int main(int argc, char *argv[]) {

// Initialize data.
int process_information = 0;
int state_information = 0;
int time = 0;
int memory = 0;
int command_line = 0;
char pid[9] = "";

// Begin handling of arguments.
handle_args(argc, argv, process_information, state_information, time, memory, command_line, pid);
return 0;
}
