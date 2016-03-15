#include <stdio.h>
#include <string.h>

#include "modbusAPDU.h"
#include "aux.h"

#define port 6666

int main(){
  int fd, rq_hand;

  fd = connectServer (port);
  if (fd < 0)
  {
    printf("Error conecting Server...\n\nTerminating...\n\n");
    return 0;
  }

  while (1)
  {
    rq_hand = Request_handler(fd);

    if (rq_hand < 0)
    {
      printf("Error: Request handler\n");
      return -1;
    }
  }

  disconnect (fd);
}
