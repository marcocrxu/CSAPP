#include <stdio.h>
#include <math.h>

int test_floatFloat2Int(unsigned uf) {
  float f = u2f(uf);
  int x = (int) f;
  return x;
}

int main() {
	if (isnan(0x7F000000)) {
		printf("it's nan");
	}
	return 0;
}
