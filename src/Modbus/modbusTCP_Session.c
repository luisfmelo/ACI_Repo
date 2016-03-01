#include "modbusTCP_Session.h"

<<<<<<< HEAD
int Create ( ){
  return 1;
}
=======
#include "modbusTCP_Session.h"
>>>>>>> 542295b36cba51b6b2bfa4b95353aea269ea2e79

int Open ( ){
  return 1;
}

int Close ( ){
  return 1;
}

int Send_Modbus_request (int fd, char* PDU, char* PDU_R)
{
  unsigned int trans_id, res, i;
  char *MBAP, *ADU;

  // generate TI (transaction ID - sequence number)
  trans_id = 0; //recebo pelo cliente? ou gero um aleatorio aqui?

 // create PDU = APDU(SDU) + MBAP -> eu acho que é ADU = MBAP + PDU
  // create MBAP = 2 bytes (trans_id) + 2 bytes (protocol_id) + 2 bytes (length) + 1 byte (unit_id)
  MBAP = (char*)malloc(7 * sizeof(char));

  // Transaction Identifier (2 bytes)
  MBAP[0] = trans_id & 0xFF;
  MBAP[1] = (trans_id >> 8) & 0xFF;
  // Protocol Identifier -> 0: MODBUS
  MBAP[2] = 0;
  MBAP[3] = 0;
  // Length: Unit Identifier (1 byte) + PDU length
  MBAP[4] = ( sizeof(PDU) + 1) & 0xFF;
  MBAP[5] = ( (sizeof(PDU) + 1) >> 8) & 0xFF;
  // Unit Identifier (1 byte)
  MBAP[6] = 0;

  //create ADU = MBAP + PDU
  ADU = (char*) malloc( sizeof(MBAP) + sizeof(PDU));

  for (i = 0; i < sizeof(MBAP); i++)
    ADU[i] = MBAP[i];

  for (i = 0; i < sizeof(MBAP); i++)
    ADU[i] = MBAP[i];







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



 // no errors
   //return nCoils;

   return 1;
}

int Receive_Modbus_request (int fd, char *APDU_P, int TI)
{
  //read (fd, PDU) // lê PDU com pedido
  /// extrai MBAP (PDUAPDU_P) e TI
  // retorna: APDU_P e TI – ok, <0 – erro
  return 1;
}

int Send_Modbus_response (int fd, char *APDU_P, int TI)
{
  // constroi PDU = APDU_R + MBAP (com TI)
  //write (fd, PDU)
  // envia Modbus TCP PDU com resposta
  // retorna: >0 – ok, <0 – erro
  return 1;
}
