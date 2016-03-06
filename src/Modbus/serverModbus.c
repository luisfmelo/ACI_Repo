#include <stdio.h>
#include <string.h>

#include "modbusAPDU.h"
#include "aux.h"

#define port 6666

int main(){
  int fd, n, n_send;
  char buf[256], send[256];

  fd = connectServer (port);
  n = read(fd,buf,255);

  if ( n < 0)
  {
    printf("ERROR - READ\n");
    return -1;
  }

  for (int i = 0; i < 7; i ++)
    send[i] = buf[i];

  if ( 1 ) //no errors  <<<<<<<<<< check condition
  {
    send[7] = 0x0F;
    send[8] = buf[8];
    send[9] = buf[9];
    send[10] = buf[10];
    send[11] = buf[11];
    n_send = 12;
  }
  else // with errors
  {
    send[7] = 0x8F;
    send[8] = 0x01; // code error
    n_send = 9;
  }

  print_hex("Client Sends: ", (unsigned char*)buf, n);
  print_hex("Client Sends: ", (unsigned char*)send, n_send);

  n_send = write(fd,buf,n_send);

  if (n < 0)
      printf("ERROR - WRITE\n");

  disconnect (fd);
}
