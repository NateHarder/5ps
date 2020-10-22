#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stat.h"
/*
Authors: Nathaniel Harder, Brendan Embrey
Assignment Number: 1
Date of Submission: 10/21/20
Name of this file: 5ps.c
Description of the program: 5ps.c takes arguments supplied from the command 
line and parses them before passing them to the parse function in stat.c file
to print the desired data. stat.c takes information from 5ps.c and searches for
data related to a given process before printing that information to stdout.
*/

/*
Function Name: check_arg
Input to the method: argument string.
Brief description of the task: checks to make sure supplied
argument has no extra characters.
*/
void check_arg(char *argument) {
    /*If argument of size two or more is selected, and it isn't a PID number,
    print an error and exit.*/
    if(argument[0] == '-' && strlen(argument) > 2) {
        argument++; // Move argument pointer to the invalid options in string.
        printf("%s not recognized .\n", argument);
        exit(0);
    }
}

/*
Function Name: check_pid_valid
Input to the method: pid string.
Brief description of the task: checks to make sure supplied
pid is numerical.
*/
void check_pid_valid(char *pid) {
    /* If no letters are selected, ensure that the argument is a valid pid. */
    for(int pid_index = 0; pid_index < strlen(pid); pid_index++) {
        if(!isdigit(pid[pid_index])) {
            // Print an error and exit if invalid.
            printf("Error: invalid PID.\n");
            exit(0);
        } 
    } 
}

/*
Function Name: handle_pid
Input to the method: pid string and argument boolean values as integers.
Brief description of the task: checks to make sure -p argument is used, at
least one other argument is used
and pid is selected before continuing.
*/
void handle_pid(char *pid, int proc, int state, int time, int mem, int cmd) {
    /* If no pid is selected as an argument, default to a pid of 1. */
    if(proc == 0) {
        pid = "1\0";
    } else {
    /* If the argument -p is used and no pid is given, exit 
    with an error message. */
        if (strcmp(pid, "") == 0) {
            printf("Error: invalid PID.\n");
            exit(0);
        }
    }
    /* If only -p is selected, print an error. */
    if (state == 0 && time == 0 && mem == 0 && cmd == 0) {
        printf("Please select at least 1 more option:\n-s,-t, or -v.\n");
        exit(0);
    }

    // Otherwise, continue to search for the data.
    move_to_dir(pid, state, time, mem, cmd);
}

/*
Function Name: handle args
Input to the method: command line arguments for -p (PID), -s (state), 
-t (time), -c (command line), -v (memory used), and a pid number, as well as
boolean values stored as integers for the command line arguments.
Output(Return value): None. 
Brief description of the task: takes command line options and interprets data 
from them before passing them to be parsed.
*/
void handle_args(int argc, char **argv, char *pid) {
    // Initialize data.
    int process = 0;
    int state = 0;
    int time = 0;
    int memory = 0;
    int command_line = 0;
    /* loop through arguments and set bool to true if the letter is found. */
    int pid_arg; // used to remember position of pid number in command line.
    for(int index = 1; index < argc; index++) {
                check_arg(argv[index]); // Check for extra characters.
                switch (argv[index][1]) {
                    case 'p':
                        process = 1;
                        if (argv[index+1]) { // Check if there is a PID.
                            check_pid_valid(argv[index+1]);
                            /* If the argument is a number, set pid to the
                            number and use pid_arg to store pid position. */
                            pid = argv[index+1];
                            pid_arg = index+1;
                        }
                        break;
                    /* If the argument is -s, -t, -c, or -v, set the boolean 
                    value for that argument to 1.*/    
                    case 's':
                        state = 1;
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
                        /* If the index is not the same as the stored pid's,
                        print an error because there is an invalid argument. */
                        if (index != pid_arg) {
                            printf("Error: %s not recognized.\n", argv[index]);
                            exit(0);
                        }
                        break; // Argument is a pid, so skip to the next one.
                }
    }
    handle_pid(pid, process, state, time, memory, command_line);
}

/*
Function Name: main
Input to the method: command line arguments for -p (PID), -s (state), 
-t (time), -c (command line), -v (memory used).
Output(Return value): returns 0 if program executes successfully. 
Brief description of the task: initializes pid to an empty string, and passes 
pid and arguments to handle_args.
*/
int main(int argc, char **argv) {
// Declare pid to be 9 characters long.
char pid[9];
// Begin handling of arguments.
handle_args(argc, argv, pid);
return 0;
}
