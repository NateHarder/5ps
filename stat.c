#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fps.h>
#include <dirent.h>
/*
Authors: Nathaniel Harder, Brendan Embrey
Assignment Number: 1
Date of Submission:
Name of this file: stat.c
Description of the program:
*/

int parse(int process_information,  char *pid, int state_information, int time, int memory, int command_line) {
    DIR *dir;
    struct dirent *d;
    chdir("/proc/" + pid);
    dir = opendir(".");
    read(dir,d);
    
    






}

int read(DIR dir, Struct dirent d) {
    char info[1024]; 
    
    if(dp = readdir(d->d_name, "stat") != NULL) {
        if(strcmp(d->d_name, "stat") == 0) {
        
        }
    }
}


int main(int argc, char *argv[]) {

}
