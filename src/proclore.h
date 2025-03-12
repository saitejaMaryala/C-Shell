#ifndef proclore_h
#define proclore_h

#include "headers.h"
#include "display.h"

pid_t get_process_group(pid_t pid);

void proclore(int pid,char* home);

#endif