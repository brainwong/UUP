#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("%s\n", getpwuid(getuid())->pw_name);
	return 0;
}
