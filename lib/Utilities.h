#include <time.h>

void wait(int msec) {
	clock_t start = clock();
	while ((clock() - start * 1000 / CLOCKS_PER_SEC) < msec);
}