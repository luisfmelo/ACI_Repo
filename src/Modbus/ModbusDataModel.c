#include "ModbusDataModel.h"

unsigned char Coils[MAX_ADDR];

void initDriver(){
  for(int i = 0; i < MAX_ADDR; i++) {
    Coils[i] = 48;
  }
}

void showActiveCoils(){
  for(int i = 0; i < MAX_ADDR; i++) {
    if(Coils[i] > 0)
      printf("%d: %d\n", i, Coils[i]);
  }
}

int W_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils){
  int i, j = 0, n = 0;

  // write nCoils with value valueCoils coils starting at startCoilAddr
  while(1) {
    int bit[8];

    for (i = 0; i < 8; i++)
      bit[i] = ((valueCoils[j] >> i) & 1);

    for (i = 0; i < 8 ; i++, n++) {
      if ( n > nCoils )
        break;

      Coils[n + startCoilAddr] = bit[i] + 48;
    }

    j ++;
    if (j * 8 >= nCoils)
      break;
  }

  // return: number of written coils – ok,  <0 – erro
  return nCoils;
}

int R_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils){
  int j = 0;
  int N = nCoils % 8 != 0 ? nCoils / 8 + 8 :
                            nCoils;
  int *bits;

  bits = (int*) malloc(sizeof(int) * N);

  // read nCoils starting at startCoilAddr and put the result at valueCoils
  while(j < nCoils) {
    if (Coils[startCoilAddr + j] == 48)
      bits[j] = 0;
    else
      bits[j] = 1;
    j++;
  }

  j = 0;

  while(j < N / 8) {
    valueCoils[j] = 0;
    for(int i = 0; i < 8; i++)
      valueCoils[j] += bits[j * 8 + i] * power(2, i);
    j++;
  }

  // returna: number of read coils, and values at valueCoils – ok,  <0 – erro
  return nCoils;
}
