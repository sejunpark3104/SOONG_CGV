#include <stdio.h>
#include <stdlib.h>
#include "movie.h"

int test(Context* context, int cmdCnt, char* cmdArg[]) {
	int str[10] = {1,};
	printf("Just for test %d %d\n", str[0], str[1]);
	return 0;
}
