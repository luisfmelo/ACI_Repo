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
  unsigned int trans_id, protocol_id[2], length, unit_id, res;
  unsigned char *MBAP, *ADU;
  unsigned char startCoilAddr[2], nCoils[2];

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


//CHECKAR A PARTIR DAQUI

 //write (fd, PDU) // envia Modbus TCP PDU
  res = W_coils (ADU);

 //read (fd, PDU_R) // resposta ou timeout
 res = R_coils (&PDU_R);

 // se resposta, remove MBAP, PDU_R  APDU_R
 // retorna: APDU_R e 0 – ok, <0 – erro (timeout)
 return 1;
 }
