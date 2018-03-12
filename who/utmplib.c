#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "utmplib.h"

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

static int utmp_reload()
{
	int amt_read;

	amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read / UTSIZE;
	cur_rec = 0;
	return num_recs;
}

int utmp_open(char *fn)
{
	fd_utmp = open(fn, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

struct utmp *utmp_next(void)
{
	struct utmp *recp;

	if (fd_utmp == -1)
		return NULLUT;

	do {
		if (cur_rec == num_recs && utmp_reload() == 0)
			return NULLUT;

		recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
		cur_rec++;

	} while (recp->ut_type != USER_PROCESS);

	return recp;
}

void utmp_close(void)
{
	if (fd_utmp != -1)
		close(fd_utmp);
}

int utmp_seek(int offset, int base)
{
	if (base == SEEK_SET) {
		if (lseek(fd_utmp, offset * UTSIZE, base) == -1)
			return -1;
	} else if (base == SEEK_END) {
		if (lseek(fd_utmp, offset * UTSIZE, base) == -1)
			return -1;
	} else if (base == SEEK_CUR) {
		if (lseek(fd_utmp, (offset - num_recs + cur_rec) * UTSIZE, base) == -1)
			return -1;
	} else {
		return -1;
	}
	cur_rec = num_recs = 0;

	return 1;
}
