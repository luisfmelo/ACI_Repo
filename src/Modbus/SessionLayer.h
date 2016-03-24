#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "aux.h"

          /* Session layer */

/**
*  Client sends to Server PDU, receiving PDU_R with informtion
*  return nCoils if no errors
*/
int Send_Modbus_request (int fd, unsigned char* SDU, unsigned char* SDU_R, int SDUsize);

/**
*  get SDU of session layer which is equals to PDU of application layer
*/
int Receive_Modbus_request (int fd, unsigned char *PDU, int *TI);

/**
*  send to client PDU of session layer
*/
int Send_Modbus_response (int fd, unsigned char *SDU, int TI, int SDU_Rsize);
