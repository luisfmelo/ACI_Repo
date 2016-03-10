#include <stdio.h>

#include "modbusAPDU.h"

#include "aux.h"

#define port 6666

int main(){
  int fd, res;
  int startCoilAddr = 20, nCoils = 9;

  fd = connectClient ("127.0.0.1", port);
  if (fd < 0)
  {
    return 0; //?????????
  }

  printf("Client started. Press any to send...\n");

  /*******************TESTE ESCRITA************************
  //  unsigned char vals[2] = {0x4C, 0x4D};
    unsigned char vals[2] = {0xCD, 0x01};

    getchar();

    res = Write_multiple_coils(fd, startCoilAddr, nCoils, vals);

    printf("Hello");
  *******************************************************/

  /*******************TESTE LEITURA************************/
  //  unsigned char vals[2] = {0x4C, 0x4D};
    unsigned char *vals;
    vals = (unsigned char*)malloc(nCoils/8 + 1);

    res = Read_coils(fd, startCoilAddr, nCoils, vals);

    print_hex("Received", vals, nCoils/8 + 1);
  /*******************************************************/
  if (res < -1)
    printf("ERRO!\n");


  disconnect (fd);
}
