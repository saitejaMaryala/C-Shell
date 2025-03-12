#ifndef prompt_h
#define prompt_h

#include "headers.h"
#include "color.h"


char *get_old_cwd();

char *get_cwd();

void get_path_of_dir(char *name,char *dir_name);

void set_cwd(char *path);

void get_curr(char *path);

void time_taken();

void set_time_name(int time_f,char* name_f);

#endif