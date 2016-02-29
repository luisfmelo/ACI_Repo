#include "tcp.h"

int W_coils (int fd, char *ADU){
  /*
  // read nCoils at some address (which start with startCoilAddr)
  unsigned char startCoilAddr[2], nCoils[2], valueCoils[nCoils];
  int n;

  bzero(valueCoils, nCoils);

  n = write(fd, valueCoils, nCoils);

  if ( n < 0)
  {
      printf("Error writing!\n");
      return -1;
  }
*/
  return 1;
  //no errors
  //return nCoils;
}

int R_coils (int fd, char* ADU)
{
  /*
    // read nCoils at some address (which start with startCoilAddr)
    unsigned char startCoilAddr[2], nCoils[2], valueCoils[nCoils];

    bzero(valueCoils, nCoils);

    n = read(fd, valueCoils, nCoils);

    if ( n < 0)
    {
        printf("Error reading!\n");
        return -1;
    }

    //no errors
    return nCoils;
    */
    return 1;
}
