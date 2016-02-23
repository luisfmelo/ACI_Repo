#include <stdio.h>

#include "modbusAPDU.h"

#define port 6666

int main(){
  int fd;
  fd = connectServer (port);
  disconnect (fd);
}
