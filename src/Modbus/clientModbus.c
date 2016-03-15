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
      /*******************TESTE ESCRITA************************/
      //  unsigned char vals[2] = {0x4C, 0x4D};
      //  unsigned char vals[2] = {0xCD, 0x01};
      printf("\nWrite:");
      scanf("%s", vals);
      res = Write_multiple_coils(fd, startCoilAddr, nCoils, vals);

        //printf("Hello");
      /*******************************************************/
    }
    else if ( c == 'r' || c == 'R')
    {
      /*******************TESTE LEITURA************************/
      //  unsigned char vals[2] = {0x4C, 0x4D};
      //  unsigned char *vals;
      //  vals = (unsigned char*)malloc(nCoils/8 + 1);

      res = Read_coils(fd, startCoilAddr, nCoils, vals);

      print_hex("Received", vals, nCoils % 8 == 0 ? nCoils/8 : nCoils/8 + 1);
      /*******************************************************/
    }

    if (res < -1)
      printf("ERRO!\n");
  }

  disconnect (fd);
}
