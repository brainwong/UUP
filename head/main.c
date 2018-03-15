#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "head.h"

int main(int argc, char **argv)
{
	FILE *fp;
	char *stropt;
	int opt, i, lines, chars;

	stropt = "+:n:c:q";

	while ( (opt = getopt(argc, argv, stropt)) != -1) {
		switch (opt) {
			case 'n':
				set_print_lines(atoi(optarg));
				set_print_chars(0);
				break;
			case 'c':
				set_print_chars(atoi(optarg));
				set_print_lines(0);
				break;
			case 'q':
				set_showfname(0);
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-q] [-n [-]K] [-c [-]K]\n", argv[0]);
				exit(1);
			case ':':
				fprintf(stderr, "%s need a parameter\n", optopt);
				exit(1);
		}
	}
	/* 
	 * only one file 
	 * disable print file nmae
	 */
	if (optind >= argc - 1)
		set_showfname(0);

	/* get input from stdin */
	if (optind == argc) {
		if (get_print_lines() > 0 || get_print_chars() > 0)
			fp = stdin;
		else
			fp = get_file_from_stdin(&lines, &chars);
		do_head(fp, NULL, lines, chars);
		exit(0);
	}

	for (i = optind; i < argc; i++) {
		lines = chars = 0;

		if (get_showfname() != 0) {
			if (strcmp(argv[i], "-") == 0)
				printf("==> %s <==\n", "standard input");
			else
				printf("==> %s <==\n", argv[i]);
		}

		if (strcmp(argv[i], "-") == 0) { /* standard input */
			if (get_print_lines() > 0 || get_print_chars() > 0)
				fp = stdin;
			else
				fp = get_file_from_stdin(&lines, &chars);
			argv[i] = "standard input";

		} else if ( (fp = fopen(argv[i], "r")) == NULL) {
			fprintf(stderr, "open %s error: %s\n", argv[i], strerror(errno));
			exit(1);
		}


		do_head(fp, argv[i], lines, chars);

		if (get_showfname() != 0 && argv[i+1] != NULL)
			printf("\n");

		if (fileno(fp) != fileno(stdin))
			fclose(fp);
		else if (!isatty(fileno(fp)))
			clear(fp);
	}

	exit(0);
}
