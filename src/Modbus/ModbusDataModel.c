#include "ModbusDataModel.h"

unsigned char Coils[MAX_ADDR] = {0};

void showActiveCoils()
{
  for(int i = 0; i < MAX_ADDR; i++) {
    if(Coils[i] > 0)
      printf("%d: %d\n", i, Coils[i]);
  }
}

int W_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  int i;

  // escreve n_c coils a partir de st_c e escreve val
  for(i = 0; i < nCoils; i++) {
    Coils[startCoilAddr + i] = valueCoils[i/8] & (1 << i%8);
  }

  // retorna: num coils escritas, valores em val – ok,  <0 – erro
  return i;
}

int R_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  int i, N;

    if (nCoils % 8 != 0)
      N = nCoils / 8 + 1;
    else
      N = nCoils / 8;

  valueCoils = (unsigned char*)malloc(N * sizeof(unsigned char));

  for(i = 0; i < nCoils; i++) {
    valueCoils[i/8] |= Coils[startCoilAddr + i] & (1 << i%8);
  }

  return i;
}
