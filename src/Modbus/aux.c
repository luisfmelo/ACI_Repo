#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aux.h"

void print_hex(char *whereAmI, unsigned char *hex, int size)
{
  printf("(size: %02d) --->%s:\t\t", size, whereAmI);
  for (int i = 0; i < size; i++)
  {
    printf("%2x ", hex[i]);
  }
  printf("\n");
}
