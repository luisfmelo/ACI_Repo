#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "SessionLayer.h"
#include "ModbusDataModel.h"

          /* Application layer Protocol Data Unit */

/**
 *  create new client socket (fd)
 *  return:    fd: ok   -1: error
 */
int connectClient (char* ip_serv, int port);

/**
 *  create new server socket (fd)
 *  return:   fd: ok   -1: error
 */
 int connectServer (int port);

/**
 *  disconect ( server or client )
 *  return:   1: ok   -1: error
 */
int disconnect (int fd);

/**
 *  Write nCoils with values: valueCoils at socket fd, starting at startCoilAddr
 *  return:  number of writed coils: ok    -1: error
 */
int Write_multiple_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils);

/**
 *  Read nCoils with values: valueCoils at socket fd, starting at startCoilAddr
 *  return: values in valueCoils
 */
int Read_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils);

/**
 *  retorna:
 */
int Request_handler (int fd);
