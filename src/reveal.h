#ifndef reveal_h
#define reveal_h

#include "headers.h"
#include "color.h"
#include "prompt.h"

#define MAX_FILES 1024

int is_hidden(char *fname);

int is_directory(char *fname);

int is_executable(char *fname);

int compare(const void *a, const void *b);

void set_color(char *fname);

bool containsOnlyAandL(char *str);

int set_hidden_and_extrainfo(char **input, int size,int* index);

void print_file_info(char *filename);

void reveal(char **input, int size, char *home);


#endif
