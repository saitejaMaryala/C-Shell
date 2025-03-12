#ifndef seek_h
#define seek_h


#include "headers.h"
#include "color.h"
#include "prompt.h"


void seek(char **cmds, int size, char *home);

void remove_extension(char *filename);

int alphasort(const struct dirent **a, const struct dirent **b);

void seek_fd(int f_flag, int d_flag, int e_flag,char* name, char *path,char *s_path,int* got_one);

int count_name(int d_flag,int f_flag,char* name,char* path);
#endif