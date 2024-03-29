#include "SessionLayer.h"

int ID = 100;

int Send_Modbus_request (int fd, unsigned char* SDU, unsigned char* SDU_R, int SDUsize){
  unsigned int trans_id, res, i, n_coils;
  unsigned char MBAP[7], PDU[7 + SDUsize], PDU_R[256];

  // generate TI (transaction ID - sequence number)
  trans_id = ID;
  ID++;

  // create PDU = APDU(SDU) + MBAP
  // create MBAP = 2 bytes (trans_id) + 2 bytes (protocol_id) + 2 bytes (length) + 1 byte (unit_id)
  // MBAP = (unsigned char*)malloc(7 * sizeof(unsigned char));

  // Transaction Identifier (2 bytes)
  MBAP[0] = (trans_id >> 8) & 0xFF;
  MBAP[1] = trans_id & 0xFF;
  // Protocol Identifier -> 0: MODBUS
  MBAP[2] = 0x00;
  MBAP[3] = 0x00;
  // Length: Unit Identifier (1 byte) + SDU length
  MBAP[4] = ( (SDUsize + 1) >> 8) & 0xFF;
  MBAP[5] = ( SDUsize + 1) & 0xFF;
  // Unit Identifier (1 byte)
  MBAP[6] = 0x77;

  // create PDU = MBAP + SDU
  // PDU = (unsigned char*) malloc(7 + sizeof(SDU));

  for (i = 0; i < 7; i++)
    PDU[i] = MBAP[i];

  for (int j = 0; j < SDUsize; j++)
    PDU[7+j] = SDU[j];

  // print_hex("PDU", PDU, 7 + SDUsize);      // for DEBUG

  // send PDU to server
  res = write(fd, PDU, 7 + SDUsize);

  // check response
  if ( res == -1 || res != (7 + SDUsize) ) {
    printf("ERROR sending PDU!\n");
    return -1;
  }

  // read from server -> PDU_R
  res = read(fd, PDU_R, 256);

  // check if TI received it's equal to TI who was sent
  if(PDU_R[0] != MBAP[0] || PDU_R[1] != MBAP[1])
    return -1;

  printf("Request response read.\n");

  // print_hex("PDU_R", PDU_R, res);      // for DEBUG

  // check response
  if ( res == -1 ) {
    write(fd, "ERROR reading PDU_R!\n", 50);
    return -1;
  }

  // remove Header from PDU_R -> return SDU_R

   for (i = 0; i < (res - 7); i++)
     SDU_R[i] = PDU_R[7 + i];

   n_coils = (unsigned int)(SDU_R[3] << 8) + (unsigned int)(SDU_R[4]);

   /***********************************************************************************************
   print_hex("SDU",SDU, sizeof(SDU));
   print_hex("Header",MBAP, 7);
   print_hex("PDU",PDU, sizeof(PDU));
   print_hex("PDU_R",PDU_R, res);
   print_hex("SDU_R",SDU_R, res - 7);
   ***********************************************************************************************/

   printf("Request successfully sent!\n");

   // no errors
   return n_coils;
}

int Receive_Modbus_request (int fd, unsigned char *SDU, int *TI){
  unsigned char PDU[256+7];
  int len;

  printf("Waiting for a request...\n");

  len = read(fd, PDU, 256+7);
  if(len < 0) {
      printf("Error reading socket!\n");
      return -1;
  }

  // PDU = Header + SDU
  // get MBAP (PDU -> PDU_P) e TI
  *TI = (int)(PDU[0] << 8) + (int)(PDU[1]);

  for(int i = 0; i < (len - 7) ; i++)
    SDU[i] = PDU[i+7];

  // returna: SDU; TI; lenth of SDU – ok, <0 – erro
  return (len-7);
}

int Send_Modbus_response (int fd, unsigned char *SDU_R, int TI, int SDU_Rsize){
  // MBAP =2 bytes (trans_id) + 2 bytes (protocol_id) + 2 bytes (length) + 1 byte (unit_id)
  // create PDU(session) = MBAP + SDU_R
  unsigned char MBAP[7];

  // Transaction Identifier (2 bytes)
  MBAP[0] = (TI >> 8) & 0xFF;
  MBAP[1] = TI & 0xFF;
  // Protocol Identifier -> 0: MODBUS
  MBAP[2] = 0x00;
  MBAP[3] = 0x00;
  // Length: Unit Identifier (1 byte) + PDU length
  MBAP[4] = ((SDU_Rsize + 1) >> 8) & 0xFF;
  MBAP[5] = (SDU_Rsize + 1) & 0xFF;
  // Unit Identifier (1 byte)
  MBAP[6] = 0x77;

  unsigned char PDU[SDU_Rsize + 7];

  int i;

  for(i = 0; i < 7; i++)
    PDU[i] = MBAP[i];

  for(int j = 0; j < SDU_Rsize; j++)
    PDU[7+j] = SDU_R[j];

  // print_hex("Response-PDU", PDU, SDU_Rsize + 7);     // for DEBUG

  // send Modbus TCP PDU with response
  int n;

  n = write(fd, PDU, SDU_Rsize + 7);

  if ( n < 0)  {
      printf("Error writing!\n");
      return -1;
  }

  return n;
}
