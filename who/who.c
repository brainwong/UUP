#include <utmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include "utmplib.h"

#define SHOWHOST
#define FINISH 1

void showtime(time_t);
int showinfo(struct utmp *, int, char *);

int main(int argc, char **argv)
{
	struct utmp *utbuf;
	int whoami;
	char *tname;

	whoami = 0;

	if (argc == 3 && strcmp(argv[1], "am") == 0 && strcmp(argv[2], "i") == 0)
		whoami = 1;
	else if (argc != 3 && argc != 1){
		fprintf(stderr, "Usage: %s [am i]\n", argv[0]);
		exit(1);
	}

	if (utmp_open(UTMP_FILE) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}
	
	tname = whoami ? ttyname(STDIN_FILENO)+5 : NULL;

	while ( (utbuf = utmp_next()) != (struct utmp *)NULL) {
		if (showinfo(utbuf, whoami, tname) == FINISH && whoami == 1)
			break;
	}

	utmp_close();
	return 0;
}

int showinfo(struct utmp *ut, int wai, char *tname)
{
	if (ut->ut_type != USER_PROCESS)
		return 0;
	if (wai == 1 && strcmp(ut->ut_line, tname) != 0)
		return 0;

	printf("%-8.8s ", ut->ut_name);

	printf("%-8.8s ", ut->ut_line);
	showtime(ut->ut_time);
#ifdef SHOWHOST
	if (ut->ut_host[0] != 0)
		printf("(%s)", ut->ut_host);
#endif
	printf("\n");

	return FINISH;

}

void showtime(time_t tval)
{
	struct tm *ts;
	char buf[30];

	ts = localtime(&tval);
	strftime(buf, 30, "%F %R ", ts);
	printf(buf);
}
