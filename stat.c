#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include "dirent.h"
#include <unistd.h>

/*
Authors: Nathaniel Harder, Brendan Embrey
Assignment Number: 1
Date of Submission:
Name of this file: stat.c
Description of the program: 
stat.c takes information from 5ps.c and searches for data related to a given process
before printing that information to stdout.
*/

/*
Function Name: format_time
Input to the method: A string for printing time in HH:MM:SS format as well as two string representing 
the utime and stime values of a process.
Output(Return value): None.
Brief description of the task: receives string values for utime and stime values in stat file,
then prints them to time_string string in format HH:MM:SS.
*/
void format_time(char *time_string, char *utime, char *stime) {
    // Convert utime and stime to integers, then divide by ticks per second to get total seconds.
    int l_utime = strtol(utime, NULL, 10);
    int l_stime = strtol(stime, NULL, 10);
    int ticks_per_second = sysconf(_SC_CLK_TCK);
    int seconds = (l_utime + l_stime) / ticks_per_second;
    /* Divide seconds by 60 to get minutes, minutes by 60 to get hours, and take seconds mod 60 to get the remaining number of seconds
    after hours and minutes have been counted. Then print the data to time_string. */
    int minutes = seconds / 60;
    int hours = minutes / 60;
    seconds = seconds % 60;
    sprintf(time_string, "%02d:%02d:%02d", hours, minutes, seconds);
}

/*
Function Name: cut_newline
Input to the method: A string that may have a newline character at the end.
Output(Return value): None.
Brief description of the task: removes a newline character from a string if present.
*/
void cut_newline(char *data_string) {
    int last = strlen(data_string) - 1;
    if(data_string[last] == '\n') {
        data_string[last] = '\0';
    }
}

/*
Function Name: replace_paren
Input to the method: A string containing data surrounded by parentheses.
Output(Return value): None.
Brief description of the task: convert parentheses in a string to brackets.
*/
void replace_paren(char *data_string) {
    /*Replace ( and ) with [ and ] then print.*/
    int last = strlen(data_string) - 1;
    data_string[0] = '[';
    data_string[last] = ']';
    printf("%s", data_string);
}

/*
Function Name: read_data
Input to the method: A string to print data to, DIR and dirent variables for navigating a directory, and a filename and field number stored as strings.
Output(Return value): None.
Brief description of the task: finds a given data point from the /proc/ directory, removes any newlines, and copies the data
into a given result_string string. 
*/
void read_data(char *result_string, DIR *dir, char *filename, char *field) {
    
    char info[1024] = "";
    struct dirent *d;
    FILE *search;
    // Use the readdir and strcmp function to check that the given file exists.
    if((d = readdir(dir)) != NULL) {
        if(strcmp(d->d_name, filename) != 0) {
            // Use popen to search for the requested data with the unix cut command,
            // then copy the data to rsult string.
            char command[300] = "";
            sprintf(command,  "cut -d \" \" -f %s %s", field, filename);
            search = popen(command, "r");
            if (fgets(info, sizeof(info), search)) {
                strcpy(result_string, info);
            } else {
                result_string = "";
            }
            pclose(search);
        } 
    }
    cut_newline(result_string); // Remove trailing newline.
}

/*
Function Name: search_data
Input to the method: DIR and dirent variables for navigating a directory as well as boolean values stored as integers
for handling command line arguments.
Output(Return value): None.
Brief description of the task: decides to search for and prints values if they have been requested by command line arguments.
*/
void search_data(DIR *dir, int state_selected, int time_selected, int memory_selected, int command_line_selected) {
    char state_string[1];
    char time_string[8]; 
    char mem_string[13];
    char cmd_string[1024] = "";
    char utime[8];
    char stime[8];
    /* If -s is selected as an argument, find the state information in field 3 of the stat file and print. */
    if(state_selected == 1) {
        read_data(state_string, dir, "stat", "3");
        printf("%s ", state_string);
    }
    /* If -t is selected as an argument, find the utime and stime information in fields 14 and 15 of the stat file,
    then format them in HH:MM:SS format and print. */
    if(time_selected == 1) {
        read_data(utime, dir, "stat", "14");
        read_data(stime, dir, "stat", "15");
        format_time(time_string, utime, stime);
        printf("time=%s ", time_string);
    }
    /* If -v is selected as an argument, find the memory information in line 1 of the statm file and print it. */  
    if(memory_selected == 1) {
        read_data(mem_string, dir, "statm", "1");
        printf("sz=%s ", mem_string);
    }
    /* If -c is selected as an argument, find the command information in line 1 of the cmdline file and print it. */
    if(command_line_selected == 1) {
        read_data(cmd_string, dir, "cmdline", "1");
        // Check if cmdline file returns empty, and read command from the second field of stat file if so.
        if (cmd_string[0] == '\0') {
            read_data(cmd_string, dir, "stat", "2");
            replace_paren(cmd_string);
        } else {
            printf("[%s]", cmd_string);
        }
    }
    printf("\n");
    closedir(dir);
}    
    
/*
Function Named move_to_dir
Input to the method: PID value stored in string as well as boolean dat stored as integer for command line options.
Output(Return value): None.
Brief description of the task: takes given data from the calling function and finds data if it has been selected to be returned
with integer arguments used as booleans.
*/
void move_to_dir(char *pid, int state_selected, int time_selected, int memory_selected, int command_line_selected) {
    DIR *dir;
    /* Declare various strings used to handle the data points that are being requested. */

    char p_dir[255];
    sprintf(p_dir, "/proc/%s", pid);
    /* Open the /proc/[pid] directory based on the pid number chosen. Exit if no process found. */
    dir = opendir(p_dir);
    if(dir == NULL) {
        // Exit program.
        printf("Error: no process with selected PID.");
        exit(0);
    }
    chdir(p_dir); // Change current directory to process directory.
    printf("%s: ", pid);
    
    search_data(dir, state_selected, time_selected, memory_selected, command_line_selected);
}
