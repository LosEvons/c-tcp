#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int check_socket(int exp, const char *msg){
	if (exp == SOCKETERROR){
		perror(msg);
		exit(1);
	}
	return exp;
}

int check_thread(int exp, const char *msg){
	if (exp != 0){
		perror(msg);
		exit(1);
	}
	return exp;
}