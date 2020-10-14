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

char* read_data(DIR *dir, struct dirent *d, char *filename, char *data) {
    char info[1024]; 
    FILE *search;
    char *search_result = malloc (sizeof (char) * 255);
    *search_result = '\0';
    if((d = readdir(dir)) != NULL) {
        if(strcmp(d->d_name, filename) != 0) {
            char command[255] = "";
            sprintf(command,  "cut -d \" \" -f %s %s", data, filename);
            search = popen(command, "r");
            while (fgets(info, sizeof(info), search)) {
                strcpy(search_result, info);
            }
        } 
    }
    char *search_result_full = search_result;
    free(search_result); 
    return search_result_full;
}



char* parse(int process_information,  char *pid, int state_information, int time, int memory, int command_line) {
    DIR *dir;
    struct dirent *d;
    char *state_string;
    char *time_string;
    char *mem_string;
    char *cmd_string;
    char p_dir[50] = "";
    sprintf(p_dir, "/proc/%s", pid);
    chdir(p_dir);
    dir = opendir(".");
    if(state_information == 1) {
        state_string = read_data(dir, d, "stat", "3");
    }
    if(time == 1) {
        const char *utime = read_data(dir, d, "stat", "14");
        const char *stime = read_data(dir, d, "stat", "15");
        int l_utime = strtol(utime, NULL, 10);
        int l_stime = strtol(stime, NULL, 10);
        int ticks_per_second = sysconf(_SC_CLK_TCK);
        int seconds = (l_utime + l_stime) / ticks_per_second;
        int minutes = seconds / 60;
        int hours = minutes / 60;
        seconds = seconds % 60;
        char time[8] = "";
        sprintf(time, "%02d:%02d:%02d", hours, minutes, seconds);
        time_string = time;
    }
    if(memory == 1) {
        mem_string = read_data(dir, d, "statm", "1");
    }
    if(command_line == 1) {
        cmd_string = read_data(dir, d, "cmdline", "1");
    }
    char *result = malloc (sizeof (char) * 255);
    *result = '\0';
    sprintf(result, "%s\t%s\t%s\t%s\n", state_string, time_string, mem_string, cmd_string);
    char *result_full = result;
    free(result);
    return result_full;
}
