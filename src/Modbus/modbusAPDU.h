#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include "modbusTCP_Session.h"


          /* Application layer Protocol Data Unit */


/*retorna:   fd: ok   -1: error                  */
int connectClient (char* ip_serv, int port);

/*retorna:   fd: ok   -1: error                  */
int connectServer (int port);

/*retorna:    1: ok   -1: error                  */
int disconnect (int fd);

/*retorna:  nºde coils escritas: ok    -1: error  */
int Write_multiple_coils(int fd, int startCoilAddr, int nCoils, char* valueCoils);

int Read_coils(int fd, int startCoilAddr, int nCoils, char* valueCoils);

int Request_handler (int fd);
