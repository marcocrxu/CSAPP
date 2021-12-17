#include <omp.h>
#define NUM_THREADS 8

int main() {
	omp_set_num_threads(NUM_THREADS);
	int a[10]; int b[10];
#pragma omp parallel for
	for (int i = 0; i < 10; i++) {
		a[i] = 2*b[i];
	}
	return 0;
}
