#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fps.h"
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


void format_time(char *time_string, char *utime, char *stime) {
    int l_utime = strtol(utime, NULL, 10);
    int l_stime = strtol(stime, NULL, 10);
    int ticks_per_second = sysconf(_SC_CLK_TCK);
    int seconds = (l_utime + l_stime) / ticks_per_second;
    int minutes = seconds / 60;
    int hours = minutes / 60;
    seconds = seconds % 60;
    sprintf(time_string, "%02d:%02d:%02d", hours, minutes, seconds);
}


void read_data(char *result_string, DIR *dir, struct dirent *d, char *filename, char *data) {
    char info[1024]; 
    FILE *search;
    if((d = readdir(dir)) != NULL) {
        if(strcmp(d->d_name, filename) != 0) {
            char command[255] = "";
            sprintf(command,  "cut -d \" \" -f %s %s", data, filename);
            search = popen(command, "r");
            while (fgets(info, sizeof(info), search)) {
                strcpy(result_string, info);
            }
        } 
    }
}



char* parse(char *result, int process_information,  char *pid, int state_information, int time, int memory, int command_line) {
    DIR *dir;
    struct dirent *d;
    char state_string[1] = "";
    char *time_string;
    char mem_string[255] = "";
    char cmd_string[255] = "";
    char utime[255] = "";
    char stime[255] = "";

    char p_dir[50] = "";
    sprintf(p_dir, "/proc/%s", pid);
    chdir(p_dir);
    dir = opendir(".");
    char *search_result; 
    if(state_information == 1) {
        read_data(state_string, dir, d, "stat", "3");
        strcat(result, state_string);
        strcat(result, "     ");
    }
    if(time == 1) {
        read_data(utime, dir, d, "stat", "14");
        read_data(stime, dir, d, "stat", "15");
        format_time(time_string, utime, stime);
        strcat(result, time_string);
        strcat(result, "     ");
    }
    if(memory == 1) {
        read_data(mem_string, dir, d, "statm", "1");
        strcat(result, mem_string);
        strcat(result, "     ");
    }
    if(command_line == 1) {
        read_data(cmd_string, dir, d, "cmdline", "1");
        strcat(result, cmd_string);
    }
    return result;
}
