#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

void error(char * msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int max(int a, int b) {
	if (a >= b)
		return a;
	else
		return b;
}

int min(int a, int b) {
	if (a <= b)
		return a;
	else
		return b;
}

void viewInt(const void *i) {
	printf("%d", *((int*)i));
}

void freeInt(void * i) {
	free((int*) i);
}

int compareInt(const void* a, const void* b) {
	if (*((int*) a) < *((int*) b))
		return 1;
	else
		return 0;
}