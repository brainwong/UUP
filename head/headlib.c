#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAXSIZE 4096

static int count_lines(FILE *, char *);
static void get_text(FILE *, int *, int *);
static void print_chars(FILE *, int , char *);
static void print_lines(FILE *, int , char *);

static int print_n_lines = 10;
static int print_n_chars = 0;
static int show_filename = 1;

static int count_lines(FILE *fp, char *fname)
{
	int cnt = 0;
	char buf[MAXSIZE];

	if (fseek(fp, 0, SEEK_SET) == -1) {
		fprintf(stderr, "fseek %s error: %s\n", fname, strerror(errno));
		exit(1);
	}
	while (fgets(buf, MAXSIZE, fp) != NULL)
		cnt++;
	if (ferror(fp)) {
		fprintf(stderr, "error for %s: %s\n", fname, strerror(errno));
		exit(1);
	}
	if (fseek(fp, 0, SEEK_SET) == -1) {
		fprintf(stderr, "fseek %s error: %s\n", fname, strerror(errno));
		exit(1);
	}

	return cnt;
}

static void print_lines(FILE *fp, int n, char *fname)
{
	int cnt;
	char buf[MAXSIZE];
	
	for (cnt = 0; cnt < n; cnt++) {
		if (fgets(buf, MAXSIZE, fp) != NULL)
			fputs(buf, stdout);
		if (ferror(fp)) {
			fprintf(stderr, "get text from %s error: %s\n", fname, strerror(errno));
			exit(1);
		}
		if (feof(fp))
			return;
	}
}

static void print_chars(FILE *fp, int n, char *fname)
{
	int i, c;

	for (i = 0; i < n; i++) {
		if ( (c = getc(fp)) != EOF)
			putc(c, stdout);
	}
}

static void get_text(FILE *fp, int *l, int *c)
{
	char buf[MAXSIZE];

	*l = *c = 0;
	while (fgets(buf, MAXSIZE, stdin) != NULL) {
		if (strrchr(buf, '\n') != NULL)
			(*l)++;
		(*c) += strlen(buf);
		fputs(buf, fp);
	}
	if (ferror(stdin)) {
		fprintf(stderr, "error for stdin: %s\n", strerror(errno));
		exit(1);
	}
	if (fseek(fp, 0, SEEK_SET) == -1) {
		fprintf(stderr, "fseek error: %s\n", strerror(errno));
		exit(1);
	}
}

void clear(FILE *fp)
{
	while (fgetc(fp) != EOF)
		;
}

void set_showfname(int i)
{
	show_filename = i;
}

int get_showfname(void)
{
	return show_filename;
}

void set_print_lines(int n)
{
	print_n_lines = n;
}

void set_print_chars(int n)
{
	print_n_chars = n;
}

FILE *get_file_from_stdin(int *lines, int *chars)
{
	FILE *fp;

	fp = tmpfile();
	get_text(fp, lines, chars);

	return fp;
}

void do_head(FILE *fp, char *this_file, int l, int c)
{
	int line_nums;
	int char_nums;
	struct stat st;
	

	if (print_n_lines > 0)
		print_lines(fp, print_n_lines, this_file);
	else if (print_n_lines < 0) {
		if (l == 0)
			line_nums = count_lines(fp, this_file);
		else
			line_nums = l;
		line_nums += print_n_lines;
		if (line_nums > 0)
			print_lines(fp, line_nums, this_file);
	} else if(print_n_chars < 0) {
		if (c == 0) {
			fstat(fileno(fp), &st);
			char_nums = st.st_size;
		} else
			char_nums = c;
		char_nums += print_n_chars;
		if (char_nums > 0)
			print_chars(fp, char_nums, this_file);
	} else {
		print_chars(fp, print_n_chars, this_file);
	}

}

int get_print_lines(void)
{
	return print_n_lines;
}

int get_print_chars(void)
{
	return print_n_chars;
}
