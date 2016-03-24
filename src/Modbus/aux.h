#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
*  print string hexadecimal format chars - Useful for DEBUG
*/
void print_hex(char *whereAmI, unsigned char *hex, int size);

/**
*  power: math function
*  return the result: base^exp
*/
int power(int base, unsigned int exp);


/**
*  clean string. All characters equal to 0x00
*/
void clean(unsigned char *vals, int n);
