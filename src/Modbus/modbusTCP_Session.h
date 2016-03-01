#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp.h"

          /* Session layer */

int Send_Modbus_request (int fd, unsigned char* APDU, unsigned char*APDU_R);
int Receive_Modbus_request (int fd, unsigned char* APDU_P, int TI);
int Send_Modbus_response (int fd, unsigned char* APDU_P, int TI);
