#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int W_coils (int fd, unsigned char* PDU);
int R_coils (int fd, unsigned char* PDU);
