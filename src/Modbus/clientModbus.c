#include <stdio.h>

#include "modbusAPDU.h"

#include "aux.h"

#define port 6666

int main(){
  int fd, res;

  fd = connectClient ("127.0.0.1", port);
  if (fd < 0)
  {
    return 0; //?????????
  }

/*******************TESTE ESCRITA************************/
//  unsigned char vals[2] = {0x4C, 0x4D};
  unsigned char vals[2] = {0xCD, 0x01};

  res = Write_multiple_coils(fd, 20, 9, vals);
/*******************************************************/
  if (res < -1)
    printf("ERRO!\n");


  disconnect (fd);
}
