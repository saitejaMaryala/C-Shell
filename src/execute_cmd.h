#ifndef execute_cmd_h
#define execute_cmd_h

#include "headers.h"
#include "function.h"


void set_store(int n);

int get_store();

void execute_cmd(char *input, char *home);

void backg_and_foreg(char *input, char *home);

int back_process_count(char *input);

void log_exec(char **cmds, int size);

char **tokenize(char *input, int *size, char *delim);

void tokenize_execute(char *input, char *home);

void remove_extra_spaces(char *str);

void execute_in_exec(char* input);





#endif
