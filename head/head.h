#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>

void set_showfname(int);
int  get_showfname(void);
void set_print_lines(int);
int get_print_lines(void);
void set_print_chars(int);
int get_print_chars(void);
FILE *get_file_from_stdin(int *, int *);
void do_head(FILE *, char *, int, int);
void clear(FILE *);

#endif
