#ifndef _UTMPLIB_H
#define _UTMPLIB_H

int utmp_open(char *);
void utmp_close(void);
struct utmp *utmp_next(void);
int utmp_seek(int, int);

#endif
