#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <string.h>

/*retorna:   fd: ok   -1: error                  */
int connectClient (char* ip_serv, int port);

/*retorna:   fd: ok   -1: error                  */
int connectServer (int port);

/*retorna:    1: ok   -1: error                  */
int disconnect (int fd);

/*
int Write_multiple_coils(fd, st_c, n_c, val);
int Read_coils(fd, st_c, n_c, val);
int Request_handler();
*/
