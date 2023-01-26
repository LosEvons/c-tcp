#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int check(int exp, const char *msg){
	if (exp == SOCKETERROR) {
		perror(msg);
		exit(1);
	}
	return exp;
}
