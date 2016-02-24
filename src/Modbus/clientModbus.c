#include <stdio.h>

#include "modbusAPDU.h"

#define port 6666

int main(){
  int fd;
  fd = connectClient ("127.0.0.1", port);
  disconnect (fd);
}
