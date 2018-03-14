#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXSIZE 4096

int print_n_lines = 10;
int print_n_chars = 0;
int show_filename = 1;

void do_head(FILE *fp, char *thisf, char *nextf);
unsigned count_lines(FILE *fp, char *fname);
void print_chars(FILE *fp, ssize_t n, char *fname);
void print_lines(FILE *fp, ssize_t n, char *fname);
void get_text(FILE *fp, ssize_t *l, ssize_t *c);

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
				show_filename = 0;
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-q] [-n [-]K] [-c [-]K]\n", argv[0]);
				exit(1);
			case ':':
				fprintf(stderr, "%s need a parameter\n", optopt);
				exit(1);
		}
	}
	
	if (argv[optind] == NULL || argv[optind][0] == '-') {
		ssize_t lines, chars;

		show_filename = 0;
		fp = tmpfile();
		get_text(fp, &lines, &chars);
		
		if ((print_n_lines != 0 && print_n_lines < 0 && (print_n_lines += lines) < 0) || 
				(print_n_chars != 0 && print_n_chars < 0 && (print_n_chars += chars) < 0))
			return 0;
		
		do_head(fp, NULL, NULL);
	} else {
		if (optind == argc - 1)  /* only one file */
			show_filename = 0;
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

void do_head(FILE *fp, char *this_file, char *next_file)
{
	int line_nums;
	int char_nums;
	struct stat st;
	
	if (show_filename != 0)
		printf("==> %s <==\n", this_file);

	if (print_n_lines > 0)
		print_lines(fp, print_n_lines, this_file);
	else if (print_n_lines < 0) {
		line_nums = count_lines(fp, this_file);
		line_nums += print_n_lines;
		if (line_nums > 0)
			print_lines(fp, line_nums, this_file);
	} else if(print_n_chars < 0) {
		fstat(fileno(fp), &st);
		char_nums = st.st_size + print_n_chars;
		if (char_nums > 0)
			print_chars(fp, char_nums, this_file);
	} else {
		print_chars(fp, print_n_chars, this_file);
	}

	if (show_filename != 0 && next_file != NULL)
		printf("\n");
}

unsigned count_lines(FILE *fp, char *fname)
{
	unsigned cnt = 0;
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

void print_lines(FILE *fp, ssize_t n, char *fname)
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

void print_chars(FILE *fp, ssize_t n, char *fname)
{
	ssize_t i;
	int c;

	for (i = 0; i < n; i++) {
		if ( (c = getc(fp)) != EOF)
			putc(c, stdout);
	}
}

void get_text(FILE *fp, ssize_t *l, ssize_t *c)
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

