#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include "tcp.h"

          /* Session layer */

int Create ( );
int Open ( );
int Close ( );
int Send_Modbus_request (int fd, char *APDU, char*APDU_R);
int Receive_Modbus_request (int fd, char *APDU_P, int TI);
int Send_Modbus_response (int fd, char *APDU_P, int TI);
