#ifndef display_h
#define display_h

#include "headers.h"
#include "color.h"

void user();

char *relative_path(char *base_path, char *target_path);

char *curr_working_dir();

void print_dir(char *home);

void user_dir(char *curr_dir);

#define PATH_MAX 4096



#endif