#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXSIZE 4096

static int print_n_lines = 10;
static int print_n_chars = 0;
static int show_filenmae = 1;

static void print_lines(FILE *fp, ssize_t n, int printlines, char *);
static void do_head(FILE *fp, char *thisf, char *nextf);

int main(int argc, char **argv)
{
	FILE *fp;
	int opt, i;
	char *stropt;

	stropt = "+:n:c:q";

	while ( (opt = getopt(argc, argv, stropt)) != -1) {
		switch (opt) {
			case 'n':
				print_n_lines = atoi(optarg);
				print_n_chars = 0;
				break;
			case 'c':
				print_n_chars = atoi(optarg);
				print_n_lines = 0;
				break;
			case 'q':
				show_filenmae = 0;
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-q] [-n K] [-c K]\n", argv[0]);
				exit(1);
			case ':':
				fprintf(stderr, "%s need a parameter\n", optopt);
				exit(1);
		}
	}

	if (argv[optind] == NULL || argv[optind][0] == '-') {
		show_filenmae = 0;
		do_head(stdin, NULL, NULL);
	} else {
		if (optind == argc - 1)
			show_filenmae = 0;
		for (i = optind; i < argc; i++) {
			if ( (fp = fopen(argv[i], "r")) == NULL) {
				fprintf(stderr, "open %s error: %s\n", argv[i], strerror(errno));
				exit(1);
			}
			do_head(fp, argv[i], argv[i+1]);
			fclose(fp);
		}
	}

	exit(0);
}

void do_head(FILE *fp, char *thisf, char *nextf)
{
	if (show_filenmae != 0)
		printf("==> %s <==\n", thisf);

	if (print_n_chars > 0)
		print_lines(fp, print_n_chars, 0, thisf);
	else
		print_lines(fp, print_n_lines, 1, thisf);

	if (show_filenmae != 0 && nextf != NULL)
		printf("\n");
}

void print_lines(FILE *fp, ssize_t n, int printlines, char *fname)
{
	int cnt;
	int c;
	char buf[MAXSIZE];
	
	if (printlines == 1) {
		for (cnt = 0; cnt < n; cnt++) {
			if (fgets(buf, MAXSIZE, fp) == NULL) {
				if (ferror(fp)) {
					fprintf(stderr, "get text from %s error: %s\n", fname, strerror(errno));
					exit(1);
				} else if (feof(fp))
					return;
			}
			fputs(buf, stdout);
		}
	} else {
		for (cnt = 0; cnt < n; cnt++)
			if ( (c = fgetc(fp)) != EOF)
				fputc(c, stdout);
	}
}
