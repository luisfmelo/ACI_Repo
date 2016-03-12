#include <stdio.h>
#include <string.h>

#include "modbusAPDU.h"
#include "aux.h"

#define port 6666

int main(){
  int fd, rq_hand;

  fd = connectServer (port);

  rq_hand = Request_handler(fd);

  if (rq_hand < 0) {
    printf("Error: Request handler\n");
    return -1;
  }

  disconnect (fd);
}
