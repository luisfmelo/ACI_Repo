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

int Send_Modbus_request (int fd, char *APDU, char*APDU_R);
