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
  int i, j = 0, n = 0;

  // escreve n_c coils a partir de st_c e escreve val
  while(1)
  {
    int bit[8];
    for (i = 0; i < 8; i++)
      bit[i] = ((valueCoils[j] >> i) & 1);

    for (i = 0; i < 8 ; i++, n++)
    {
      if ( n > nCoils )
        break;
      Coils[n + startCoilAddr] = bit[i] + 48;
    }

    j ++;
    if (j * 8 >= nCoils)
      break;
  }

  for(i = 0; i < 20; i++)
    printf("%d: %c\n", i, Coils[i]);

  // retorna: num coils escritas, valores em val – ok,  <0 – erro
  return nCoils;
}

int R_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  int i, N;

    if (nCoils % 8 != 0)
      N = nCoils / 8 + 1;
    else
      N = nCoils / 8;

  valueCoils = (unsigned char*)malloc(N * sizeof(unsigned char));

  for(i = 0; i < nCoils; i++)
    valueCoils[i/8] |= Coils[startCoilAddr + i] & (1 << i%8);

    valueCoils[0] = 0x03;
  return i;
}
