#include <stdio.h>
#include <stdlib.h>
#include "./include/util.h"

void errif(bool flag, const char *error_message) {
    if(flag) {
        fprintf(stderr, error_message, " is error\n");
        exit(1);
    }
}