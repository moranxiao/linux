#include <stdio.h>
void main() {
	for (int i = 0; i < 2; i++) {
		printf("outer: i=%d\n", i);
		for (int j = 0; j < 2; j++) {
			if (i != j)break;
			printf("inner: j=%d\n", j);
		}
	}
	printf("game over!");
}
