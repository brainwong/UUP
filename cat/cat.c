#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MAXSIZE 4096

int main(int argc, char **argv)
{
	FILE *fp;
	char buf[MAXSIZE];
	int i;

	if (argc == 1)
		fp = stdin;
	else {
		for (i = 1; i < argc; i++) {
			if ( (fp = fopen(argv[i], "r")) == NULL) {
				fprintf(stderr, "fopen %s error: %s\n", argv[i], strerror(errno));
				exit(1);
			}
			while (fgets(buf, MAXSIZE, fp) != NULL)
				fputs(buf, stdout);
			if (fclose(fp) == -1) {
				fprintf(stderr, "fclose %s error: %s\n", argv[i], strerror(errno));
				exit(1);
			}
		}
	}

	return 0;
}
