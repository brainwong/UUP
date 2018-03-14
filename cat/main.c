#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "cat.h"

int main(int argc, char **argv)
{
	FILE *fp;
	int opt, i;
	char *stropt;

	stropt = "+:nbesT";

	while ( (opt = getopt(argc, argv, stropt)) != -1) {
		switch (opt) {
			case 'T':
				set_fl(DT);
				break;
			case 'b':
				set_fl(SNELNUM);
				break;
			case 'e':
				set_fl(EWDOLLAR);
				break;
			case 'n':
				set_fl(SALNUM);
				break;
			case 's':
				set_fl(SREOL);
				break;
			case '?':
				fprintf(stderr, "Unknown option: %c\n", optopt);
				exit(1);
			case ':':
				fprintf(stderr, "%c need a parameter\n", optopt);
				exit(1);
		}
	}

	if (argv[optind] == NULL) {
		do_cat(stdin);
	} else {
		for (i = optind; i < argc; i++) {
			if ( (fp = fopen(argv[i], "r")) == NULL) {
				fprintf(stderr, "open %s error: %s\n", argv[i], strerror(errno));
				exit(1);
			}
			do_cat(fp);
			fclose(fp);
		}
	}

	exit(0);
}
