#ifndef _CATLIB_H
#define _CATLIB_H
#include <stdio.h>
#include <string.h>
#include "cat.h"

#define MAXSIZE 4096

static unsigned long FL_OPT;

/*
 * -1: ignore this flag
 *  0: no
 *	1: yes
 */
static return_emptyline;
static display_tab;

static void print_line(char *);
static char *get_line(FILE *, char *, size_t);

/* without any option */
static void print_file(FILE *);

/* option cat */
static void do_b_cat(FILE *);
static void do_n_cat(FILE *);

#endif
