//Create ( );
//Open ( );
//Close ( );
/*

Receive_Modbus_request ( );
Send_Modbus_response ( );
*/

#include "modbusSession.h"

int Send_Modbus_request (int fd, char *PDU, char *PDU_R)
{
  unsigned int trans_id, protocol_id[2], length, unit_id, res, i;
  unsigned char *MBAP, *ADU;

  // get start addr; nCoils and Value
  startCoilAddr[i] = PDU[i + 1];

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
  res = W_coils (fd, ADU);

 //read (fd, PDU_R) // resposta ou timeout
 res = R_coils (fd, &PDU_R);

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

 // retorna: APDU_R e 0 – ok, <0 – erro (timeout)
 return 1;
 }
