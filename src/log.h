#ifndef log_h
#define log_h

#define Max_FILES 15

#include "headers.h"


void set_history_path(char *path);

int count_lines();

void show_past_events();

void purge();

char *history_cmd(int ln);

int is_integer(const char *str);

void delete_first_line();

void store_events(char *data);

#endif