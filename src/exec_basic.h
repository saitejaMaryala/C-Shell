#ifndef exec_basic_h
#define exec_basic_h

#include "headers.h"

void remove_first_space(char *str);

char **split_string(char *str, char delimiter);

void remove_char(char *str, char ch);

void exec_echo(char* str);

void execute_basic_cmd(char *org_string,char **input,int size);

#endif