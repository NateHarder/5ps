/*
Authors: Nathaniel Harder, Brendan Embrey
Assignment Number: 1
Date of Submission: 10/21/20
Name of this file: stat.h
Description of the program: Used to link the move_to_dir function from stat.c
to be used in 5ps.c
*/

#ifndef _STAT_H
#define _STAT_H

void move_to_dir(char *pid, int state_information, int time, int memory, int command_line);

#endif // _STAT_H