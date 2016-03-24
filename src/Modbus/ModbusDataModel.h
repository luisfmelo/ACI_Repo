#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

#define MAX_ADDR 0xFFFF

void initDriver();
/**
*  show ON coils
*/
void showActiveCoils();

/**
*  Write coils at Driver
*/
int W_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils);

/**
*  Read coils from Driver
*/
int R_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils);
