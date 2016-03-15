#include "ModbusDataModel.h"
#include "aux.h"

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

  for(i = 0; i < 20; i++)
    printf("%d: %c\n", i, Coils[i]);

  // retorna: num coils escritas, valores em val – ok,  <0 – erro
  return nCoils;
}

int R_coils(int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  int j = 0;
  int N = nCoils % 8 != 0 ? nCoils / 8 + 8 :
                            nCoils;

  int *bits;

  bits = (int*) malloc(sizeof(int) * N);

  // le n_coils a partir de startCoilAddr e escreve em valueCoils - retorna N bytes lidos
  while(j < N) {
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

  // retorna: num coils escritas, valores em val – ok,  <0 – erro
  return nCoils;
}
