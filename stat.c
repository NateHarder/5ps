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

void cut_newline(char *data_string) {
    int last = strlen(data_string) - 1;
    if(data_string[last] == '\n') {
        data_string[last] = '\0';
    }
}


void read_data(char *result_string, DIR *dir, struct dirent *d, char *filename, char *data) {
    char info[1024] = "";
    FILE *search;
    if((d = readdir(dir)) != NULL) {
        if(strcmp(d->d_name, filename) != 0) {
            char command[300] = "";
            sprintf(command,  "cut -d \" \" -f %s %s", data, filename);
            search = popen(command, "r");
            while (fgets(info, sizeof(info), search)) {
                strcpy(result_string, info);
            }
        } 
    }
    cut_newline(result_string);
}



void parse(char *pid, int state_information, int time, int memory, int command_line) {
    DIR *dir;
    struct dirent *d;
    char state_string[1];
    char time_string[8];
    char mem_string[13];
    char cmd_string[1024];
    char utime[8];
    char stime[8];
    char p_dir[255];
    sprintf(p_dir, "/proc/%s", pid);
    chdir(p_dir);
    dir = opendir(".");
    if(state_information == 1) {
        read_data(state_string, dir, d, "stat", "3");
    }
    if(time == 1) {
        read_data(utime, dir, d, "stat", "14");
        read_data(stime, dir, d, "stat", "15");
        format_time(time_string, utime, stime);
    }
    if(memory == 1) {
        read_data(mem_string, dir, d, "statm", "1");
    }
    if(command_line == 1) {
        read_data(cmd_string, dir, d, "cmdline", "1");
    }

    fprintf(stdout, "%s: %c time=%s sz=%s [%s]\n", pid, state_string[0], time_string, mem_string, cmd_string);
}
