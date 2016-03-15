#include <stdio.h>

#include "modbusAPDU.h"

#include "aux.h"

#define port 6666

int main(){
  int fd, res;
  int startCoilAddr, nCoils;
  char c ;
  unsigned char vals[260];

  fd = connectClient ("127.0.0.1", port);
  if (fd < 0)
  {
    printf("Error conecting Client...\n\nTerminating...\n\n");
    return 0;
  }

  while(1)
  {
    printf("\nClient started. Press:\n\t(W)rite\n\t(R)ead\n\t(E)xit\n");
    scanf(" %c", &c);
    if ( c == 'E' || c == 'e')
      break;

    printf("\nNumber of Coils: ");
    scanf("%d", &nCoils);
    printf("\nStarting at: ");
    scanf("%d", &startCoilAddr);


    if ( c == 'w' || c == 'W')
    {
      //clean(vals, res);
      printf("\nWrite:");
      scanf("%s", vals);
      res = Write_multiple_coils(fd, startCoilAddr, nCoils, vals);
    }
    else if ( c == 'r' || c == 'R')
    {
      //clean(vals, res);
      unsigned char c[260];
      res = Read_coils(fd, startCoilAddr, nCoils, c);
      print_hex("Received", c, nCoils % 8 == 0 ? nCoils/8 : nCoils/8 + 1);
    }

    if (res < -1)
      printf("ERRO!\n");
  }

  disconnect (fd);
}
