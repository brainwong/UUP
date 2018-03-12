#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

static int overwrite = 1;

int main(int argc, char **argv)
{
	int in_fd, out_fd;
	char *input_file, *output_file;
	struct stat in_st, out_st;
	ssize_t nread;
	char buf[BUFFERSIZE];

	if (!(argc == 3 || (argc == 4 && strcmp(argv[1], "-i") == 0))) {
		fprintf(stderr, "Usage: %s [-i] source destination\n");
		exit(1);
	}
	if (argc == 4) {
		overwrite = -1;
		input_file = argv[2];
		output_file = argv[3];
	} else {
		input_file = argv[1];
		output_file = argv[2];
	}

	if (access(output_file, F_OK) == 0) {
		if (stat(input_file, &in_st) < 0) {
			fprintf(stderr, "stat %s error: %s\n", input_file, strerror(errno));
			exit(1);
		}
		if (stat(output_file, &out_st) < 0) {
			fprintf(stderr, "stat %s error: %s\n", output_file, strerror(errno));
			exit(1);
		}
		if (in_st.st_ino == out_st.st_ino) {
			fprintf(stderr, "cp: '%s' and '%s' are the same file\n", input_file, output_file);
			exit(1);
		}
		if (overwrite == -1) {
			printf("%s overwrite '%s' ? ", argv[0], output_file);
			switch(getchar()) {
				case 'y':
					break;
				default:
					return 0;
			}
		}
	}

	if ( (in_fd = open(input_file, O_RDONLY)) < 0) {
		fprintf(stderr, "open %s error: %s\n", input_file, strerror(errno));
		exit(1);
	}

	if ( (out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, COPYMODE)) < 0) {
		fprintf(stderr, "open %s error: %s\n", output_file, strerror(errno));
		exit(1);
	}

	while ( (nread = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write(out_fd, buf, nread) != nread) {
			fprintf(stderr, "write %s error: %s", output_file, strerror(errno));
			exit(1);
		}
	if (nread == -1) {
		fprintf(stderr, "read %s error: %s\n", output_file, strerror(errno));
		exit(1);
	}

	if (close(in_fd) == -1 || close(out_fd) == -1) {
		fprintf(stderr, "close error: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}
