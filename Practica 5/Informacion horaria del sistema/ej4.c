#include <time.h>
#include <sys/time.h>
#include <stdio.h>

int main() {

	int iterations = 10000;

	struct timeval start, end;
	
	gettimeofday(&start, NULL);
	int i;
	for(i = 0; i < iterations; i++) {
	}

	gettimeofday(&end, NULL);

	printf("%ld\n", ((end.tv_sec*iterations + end.tv_usec) - (start.tv_sec*iterations  + start.tv_usec)));

}
