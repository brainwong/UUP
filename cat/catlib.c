#include "catlib.h"

void set_fl(unsigned long flag)
{
	FL_OPT |= flag;
}

void do_cat(FILE *fp)
{
	if (FL_OPT & DT)
		display_tab = 1;
	else
		display_tab = 0;

	if (FL_OPT & SREOL)
		return_emptyline = 0;
	else 
		return_emptyline = -1;

	if ((FL_OPT & SNELNUM) != 0) {    /* !!!!!! */
		do_b_cat(fp);
	} else if (FL_OPT & SALNUM) {
		do_n_cat(fp);
	} else {
		print_file(fp);
	}
}


static char *get_line(FILE *fp, char *buf, size_t size)
{
	char *nlpos;

	while (fgets(buf, size, fp) != NULL) {
		nlpos = strrchr(buf, '\n');
		*nlpos = 0;
		if (return_emptyline == -1)
			return buf;
		else {
			if (buf[0] == 0 && return_emptyline == 0) {
				return_emptyline = 1;
				return buf;
			} else if (buf[0] != 0) {
				return_emptyline = 0;
				return buf;
			}
		}
	}
	return NULL;
}

static void print_line(char *buf)
{
	char *strp;

	strp = buf;
	while (*strp != 0) {
		if (display_tab == 1 && *strp == '\t')
			printf("^I");
		else
			printf("%c", *strp);
		strp++;
	}

	if (FL_OPT & EWDOLLAR)
		fputs("$\n", stdout);
	else 
		fputs("\n", stdout);
}

static void print_file(FILE *fp)
{
	char buf[MAXSIZE];

	while (get_line(fp, buf, MAXSIZE) != NULL)
		print_line(buf);
}


static void do_b_cat(FILE *fp)
{
	char buf[MAXSIZE];
	unsigned cnt = 0;

	while (get_line(fp, buf, MAXSIZE) != NULL) {
		if (buf[0] != 0)
			printf("%6d  ", ++cnt);
		print_line(buf);
	}
}
static void do_n_cat(FILE *fp)
{
	char buf[MAXSIZE];
	unsigned cnt = 0;

	while (get_line(fp, buf, MAXSIZE) != NULL) {
		printf("%6d  ", ++cnt);
		print_line(buf);
	}
}
