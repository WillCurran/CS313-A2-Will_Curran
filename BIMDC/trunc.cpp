#include <unistd.h>

int main(int argc, char *argv[]) {
	truncate("binary_file", 100000);
	return 0;
}
