#include <stdio.h>

#include "modbusAPDU.h"

#define port 6666

int main(){
  int fd, res;

  fd = connectClient ("127.0.0.1", port);
  if (fd < 0)
  {
    return 0; //?????????
  }

  unsigned char vals[2] = {0x01, 0xCD};

  res = Write_multiple_coils(fd, 20, 9, vals);
  if (res < -1)
    printf("ERRO!\n");

  disconnect (fd);
}
