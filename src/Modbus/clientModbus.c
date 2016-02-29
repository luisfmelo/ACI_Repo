#include <stdio.h>

#include "modbusAPDU.h"

#define port 6666

int main(){
  int fd;
  fd = connectClient ("127.0.0.1", port);
  int n = Write_multiple_coils(fd, 0, 5, "abcde");
  if (n < 0)
    printf("ERROR!\n");
  else
    printf("I think it went ok!\n\n");
  disconnect (fd);
}
