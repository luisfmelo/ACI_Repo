#include <stdio.h>
#include <stdlib.h>

#define MAX_ADDR 0xFFFF

void showActiveCoils();
int W_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils);
int R_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils);
