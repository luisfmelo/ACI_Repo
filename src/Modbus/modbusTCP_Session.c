//Create ( );
//Open ( );
//Close ( );
/*

Receive_Modbus_request ( );
Send_Modbus_response ( );
*/

#include "modbusSession.h"


int Send_Modbus_request (int fd, APDU, APDU_R)
 {
 // gera TI (trans.ID número de sequência)
 // constroi PDU = APDU(SDU) + MBAP
 //write (fd, PDU) // envia Modbus TCP PDU
 //read (fd, PDU_R) // resposta ou timeout
 // se resposta, remove MBAP, PDU_R  APDU_R
 // retorna: APDU_R e 0 – ok, <0 – erro (timeout)
 return 1;
 }
