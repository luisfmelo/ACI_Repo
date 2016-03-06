#include "modbusTCP_Session.h"

int ID = 0;

/**
 *
 */
int Send_Modbus_request (int fd, unsigned char* SDU, unsigned char* SDU_R)
{
  unsigned int trans_id, res, i, n_coils;
  unsigned char MBAP[7], PDU[7 + sizeof(SDU)], PDU_R[5];

  // generate TI (transaction ID - sequence number)
  trans_id = ID;
  ID++;

  // create PDU = APDU(SDU) + MBAP
  // create MBAP = 2 bytes (trans_id) + 2 bytes (protocol_id) + 2 bytes (length) + 1 byte (unit_id)
  //MBAP = (unsigned char*)malloc(7 * sizeof(unsigned char));

  // Transaction Identifier (2 bytes)
  MBAP[0] = (trans_id >> 8) & 0xFF;
  MBAP[1] = trans_id & 0xFF;
  // Protocol Identifier -> 0: MODBUS
  MBAP[2] = 0x00;
  MBAP[3] = 0x00;
  // Length: Unit Identifier (1 byte) + SDU length
  MBAP[4] = ( (sizeof(SDU) + 1) >> 8) & 0xFF;
  MBAP[5] = ( sizeof(SDU) + 1) & 0xFF;
  // Unit Identifier (1 byte)
  MBAP[6] = 0xff;

  //create PDU = MBAP + SDU
  //PDU = (unsigned char*) malloc(7 + sizeof(SDU));

  for (i = 0; i < 7; i++)
    PDU[i] = MBAP[i];

  for (int j = 0; j < sizeof(PDU); j++)
    PDU[7+j] = SDU[j];

  // envia Modbus TCP PDU
  res = write(fd, PDU, sizeof(PDU));

  // check response
  if ( res == -1 || res != sizeof(PDU) )
  {
    printf("ERROR sending ADU!\n");
    write(fd, "ERROR sending ADU!\n", 50);
    return -1;
  }

  // lê Modbus TCP PDU
  //PDU_R = (unsigned char*)malloc(5);
  res = read(fd, PDU_R, 12);

 // check response
  if ( res == -1 )
  {
    write(fd, "ERROR reading ADU_R!\n", 50);
    return -1;
  }

  // remove Header from PDU_R -> return SDU_R
    SDU_R = (unsigned char*)malloc( res - 7 );

   for (i = 0; i < res; i++)
     SDU_R[i] = PDU_R[7 + i];

   n_coils = (unsigned int)(SDU_R[3] << 8) + (unsigned int)(SDU_R[4]);

   /***********************************************************************************************/
   print_hex("SDU",SDU, sizeof(SDU));
   print_hex("Header",MBAP, 7);
   print_hex("PDU",PDU, sizeof(PDU));
   print_hex("PDU_R",PDU_R, res);
   print_hex("SDU_R",SDU_R, res - 7);

   /***********************************************************************************************/

   // no errors
   return n_coils;
}

/**
 *
 */
int Receive_Modbus_request (int fd, unsigned char *PDU, int *TI)
{
  unsigned char ADU[1];
  int res;

  res = readSocket(fd, ADU);  // lê PDU com pedido
  if (res < 0)  {
    //ERRO
      printf("Erro a receber...");
  }

  // extrai MBAP (PDU -> APDU_P) e TI
  *TI = (int)(PDU[0] << 8) + (int)(PDU[1]);

  PDU = (unsigned char*)malloc((sizeof(ADU) - 7) * sizeof(unsigned char));

  for(int i = 0; i < sizeof(PDU) ; i++)
    PDU[i] = ADU[i+7];

  // retorna: ADU e TI – ok, <0 – erro
  return 1;
}

/**
 *
 */
int Send_Modbus_response (int fd, unsigned char *APDU_R, int TI)
{
  // constroi PDU = APDU_R + MBAP[= 2 bytes (trans_id) + 2 bytes (protocol_id) + 2 bytes (length) + 1 byte (unit_id)] (com TI)
  unsigned char * MBAP = (unsigned char*)malloc(7 * sizeof(unsigned char));

  // Transaction Identifier (2 bytes)
  MBAP[0] = (TI >> 8) & 0xFF;
  MBAP[1] = TI & 0xFF;
  // Protocol Identifier -> 0: MODBUS
  MBAP[2] = 0x00;
  MBAP[3] = 0x00;
  // Length: Unit Identifier (1 byte) + PDU length
  MBAP[4] = ((sizeof(APDU_R) + 1) >> 8) & 0xFF;
  MBAP[5] = (sizeof(APDU_R) + 1) & 0xFF;
  // Unit Identifier (1 byte)
  MBAP[6] = 0x01;

  unsigned char * PDU = (unsigned char*)malloc((sizeof(APDU_R) + 7) * sizeof(unsigned char));

  int i;

  for(i = 0; i < sizeof(MBAP); i++)
    PDU[i] = MBAP[i];

  for(int j = 0; j < sizeof(APDU_R); j++)
      PDU[i+j] = APDU_R[j];

  // envia Modbus TCP PDU com resposta
  // retorna: >0 – ok, <0 – erro
  return writeSocket(fd, PDU);
}

/**
 *
 */
int readSocket (int fd, unsigned char* ADU)
{
  int n;
  unsigned char buff[260];

  n = read(fd, buff, 260);

  if(n < 0) {
      printf("Error reading socket!\n");
      return -1;
  }

  ADU = (unsigned char *)realloc(ADU, n * sizeof(unsigned char));

  for(int i = 0; i < n; i++)
    ADU[i] = buff[i];

  //no errors
  return n;
}

/**
 *
 */
int writeSocket (int fd, unsigned char* PDU)
{
  int n;

  n = write(fd, PDU, sizeof(PDU));

  if ( n < 0)  {
      printf("Error writing!\n");
      return -1;
  }

  return n;
}
