#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char *stropt;
	int opt;

	stropt = "+:h::x:yabcdefg";
	while ( (opt = getopt(argc, argv, stropt)) != -1) {
		switch (opt) {
			default:
				printf("opt: %c\n", opt);
				break;
			case '?':
			case ':':
				fprintf(stderr, "usage error\n");
				break;
		}
	}

	int i;
	for (i = optind; i < argc; i++) {
		printf("argv[%d]: %s\n", i, argv[i]);
	}


	return 0;
}
