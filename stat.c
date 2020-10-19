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
*/

/*
format_time: receives string values for utime and stime values in stat file,
then prints them to time_string string in format HH:MM:SS.
*/
void format_time(char *time_string, char *utime, char *stime) {
    // Convert utime and stime to integers, then divide by ticks per second to get total seconds.
    int l_utime = strtol(utime, NULL, 10);
    int l_stime = strtol(stime, NULL, 10);
    int ticks_per_second = sysconf(_SC_CLK_TCK);
    int seconds = (l_utime + l_stime) / ticks_per_second;
    // Divide seconds by 60 to get minutes, minutes by 60 to get hours, and take seconds mod 60 to get the remaining number of seconds
    // after hours and minutes have been counted. Then print the data to time_string. 
    int minutes = seconds / 60;
    int hours = minutes / 60;
    seconds = seconds % 60;
    sprintf(time_string, "%02d:%02d:%02d", hours, minutes, seconds);
}


/*
cut_newline: removes a newline character from a string if present.
*/
void cut_newline(char *data_string) {
    int last = strlen(data_string) - 1;
    if(data_string[last] == '\n') {
        data_string[last] = '\0';
    }
}



/*
read_data: Finds a given data point from the /proc/ directory, removes and newlines, and copies the data
into a given result_string string. 
*/
void read_data(char *result_string, DIR *dir, struct dirent *d, char *filename, char *data) {
    
    char info[1024] = "";
    FILE *search;
    // Use the readdir and strcmp function to check that the given file exists.
    if((d = readdir(dir)) != NULL) {
        if(strcmp(d->d_name, filename) != 0) {
            // Use popen to search for the requested data with the unix cut command,
            // then copy the data to rsult string.
            char command[300] = "";
            sprintf(command,  "cut -d \" \" -f %s %s", data, filename);
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
parse: Takes given data from the calling function and finds data if it has been selected to be returned
with integer arguments used as booleans.
*/
void parse(char *pid, int state_selected, int time_selected, int memory_selected, int command_line_selected) {
    DIR *dir;
    // Declare various strings used to handle the data points that are being requested.
    struct dirent *d;
    char state_string[1];
    char time_string[8]; 
    char mem_string[13];
    char cmd_string[1024] = "";
    char utime[8];
    char stime[8];
    char p_dir[255];
    
    
    // Open the /proc/[pid] directory based on the pid number chosen. Exit if no process found.
    sprintf(p_dir, "/proc/%s", pid);
    dir = opendir(p_dir);
    if(dir == NULL) {
        // Exit program.
        fprintf(stdout, "Error: no process with selected PID.");
        closedir(dir);
        exit(0);
    } else {
        chdir(p_dir); // Change current directory to process directory.
    }

    char buff[1024];
    getcwd( buff, 1024);
    fprintf(stdout,"Current working dir: %s\n", buff);

    fprintf(stdout, "%s: ", pid);
    // If -s is selected as an argument, find the state information in field 3 of the stat file and print.
    if(state_selected == 1) {
        read_data(state_string, dir, d, "stat", "3");
        fprintf(stdout, "%s ", state_string);
    }
    // If -t is selected as an argument, find the utime and stime information in fields 14 and 15 of the stat file,
    // then format them in HH:MM:SS format and print.
    if(time_selected == 1) {
        read_data(utime, dir, d, "stat", "14");
        read_data(stime, dir, d, "stat", "15");
        format_time(time_string, utime, stime);
        fprintf(stdout, "time=%s ", time_string);
    }
    if(memory_selected == 1) {
        read_data(mem_string, dir, d, "statm", "1");
        fprintf(stdout, "sz=%s ", mem_string);
    }
    if(command_line_selected == 1) {
        read_data(cmd_string, dir, d, "cmdline", "1");
        fprintf(stdout, "[%s]", cmd_string);
    }
    closedir(dir);
}
