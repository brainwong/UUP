#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <utmp.h>
#include <stdio.h>
#include <errno.h>
#include "utmplib.h"

int logout_tty(char *line)
{
	int fd, len, retval;
	struct utmp *recp;

	len = sizeof(struct utmp);
	retval = -1;
	if ( (fd = utmp_open(UTMP_FILE)) == -1) {
		fprintf(stderr, "open error: %s\n", strerror(errno));
		return -1;
	}

	while ( (recp = utmp_next()) != (struct utmp *)NULL) {
		if (strncmp(recp->ut_line, line, sizeof(recp->ut_line)) == 0) {
			recp->ut_type = DEAD_PROCESS;
			if (time(&recp->ut_time) != -1) {
				if (utmp_seek(-1, SEEK_CUR) != -1) {
					if (write(fd, recp, len) == len) {
						retval = 0;
					} else {
						fprintf(stderr, "write error: %s\n", strerror(errno));
					}
				} else {
					fprintf(stderr, "utmp_seek error\n");
				}
			} else {
				fprintf(stderr, "time error: %s\n", strerror(errno));
				break;
			}
			break;
		}
	}
	utmp_close();
	return retval;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		exit(1);

	if (logout_tty(argv[1]) == -1) {
		fprintf(stderr, "logout_error\n");
		return -1;
	}
}
