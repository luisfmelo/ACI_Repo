#include "modbusTCP_Session.h"

int ID = 0;

int Send_Modbus_request (int fd, unsigned char* PDU, unsigned char* PDU_R)
{
  /*For Test Sake:*********************
  printf("\n\nPDU: ");
  for (int i = 0; i < sizeof(PDU) ; i++)
    printf("%x ", PDU[i]);

  PDU_R[0] = PDU[0];
  PDU_R[1] = PDU[1];
  PDU_R[2] = PDU[2];
  PDU_R[3] = PDU[3];
  PDU_R[4] = PDU[4];
  unsigned int n = (unsigned int)(PDU_R[3] << 8) + (unsigned int)(PDU_R[4]);

  printf("\n\nPDU_R: ");
  for (int i = 0; i < 5; i++)
    printf("%x ", PDU_R[i]);

  printf("\n\n\tN coils: %d", n);

  return n;
  ***********************************/


  unsigned int trans_id, res, i, n_coils;
  unsigned char *MBAP, *ADU;

  // generate TI (transaction ID - sequence number)
  trans_id = ID;
  ID++;

 // create PDU = APDU(SDU) + MBAP -> eu acho que é ADU = MBAP + PDU
  // create MBAP = 2 bytes (trans_id) + 2 bytes (protocol_id) + 2 bytes (length) + 1 byte (unit_id)
  MBAP = (unsigned char*)malloc(7 * sizeof(unsigned char));

  // Transaction Identifier (2 bytes)
  MBAP[0] = (trans_id >> 8) & 0xFF;
  MBAP[1] = trans_id & 0xFF;
  // Protocol Identifier -> 0: MODBUS
  MBAP[2] = 0x00;
  MBAP[3] = 0x00;
  // Length: Unit Identifier (1 byte) + PDU length
  MBAP[4] = ( (sizeof(PDU) + 1) >> 8) & 0xFF;
  MBAP[5] = ( sizeof(PDU) + 1) & 0xFF;
  // Unit Identifier (1 byte)
  MBAP[6] = 0xFF;

  //create ADU = MBAP + PDU
  ADU = (unsigned char*) malloc( sizeof(MBAP) + sizeof(PDU));

  for (i = 0; i < sizeof(MBAP); i++)
    ADU[i] = MBAP[i];

  for (i = 0; i < sizeof(MBAP); i++)
    ADU[i] = MBAP[i];





    printf("--------------------------------------------------------------");

 //write (fd, PDU) // envia Modbus TCP PDU
  res = W_coils (fd, PDU);

  // check response
    if ( res == -1)
    {
      printf("Error sending Modbus Request - timeout");
      return -1;
    }
    //ou else if? se for timeout da sempre erro?
    if ( PDU_R[0] == 0x8F )
    {
      printf("Error sending Modbus Request - Error: %c", PDU_R[1]);
      return -1;
    }


 //read (fd, PDU_R) // resposta ou timeout
  res = R_coils (fd, PDU_R);

 // check response
   if ( res == -1)
   {
     printf("Error sending Modbus Request - timeout");
     return -1;
   }
   //ou else if? se for timeout da sempre erro?
   if ( PDU_R[0] == 0x8F )
   {
     printf("Error sending Modbus Request - Error: %c", PDU_R[1]);
     return -1;
   }


  n_coils = (unsigned int)(PDU_R[3] << 8) + (unsigned int)(PDU_R[4]);

 // no errors
   return n_coils;
}

int Receive_Modbus_request (int fd, unsigned char *APDU_P, int TI)
{
  //read (fd, PDU) // lê PDU com pedido
  /// extrai MBAP (PDUAPDU_P) e TI
  // retorna: APDU_P e TI – ok, <0 – erro
  return 1;
}

int Send_Modbus_response (int fd, unsigned char *APDU_P, int TI)
{
  // constroi PDU = APDU_R + MBAP (com TI)
  //write (fd, PDU)
  // envia Modbus TCP PDU com resposta
  // retorna: >0 – ok, <0 – erro
  return 1;
}
