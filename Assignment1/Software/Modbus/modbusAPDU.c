#include "modbusAPDU.h"

int connectClient (char* ip_serv, int port)
{
  int fd, err;
  struct sockaddr_in serv_addr;  //endereços de cliente e servidor
  struct in_addr addr;
  fd = socket(AF_INET, SOCK_STREAM, 0);  //cria um novo socket

  if (fd < 0) {
    printf("ERROR opening socket\n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;                      //endereço da familia - sempre AF_INET
  serv_addr.sin_port = htons(port);                    //Numero da porta - #DEFINE
  inet_aton(ip_serv, &addr);                           //IP da maquina - INADDR_ANY faz isso por nos
  serv_addr.sin_addr = addr;


  err = connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if (err < 0) {
    printf("ERROR connecting\n");
    return -1;
  }

  printf("Conected with success to socket: %d!\n", fd);

  return fd;
}

int connectServer (int port)
{
  int fd, newfd, err;
  struct sockaddr_in serv_addr, cli_addr;        //endereços de cliente e servidor

  fd = socket(AF_INET, SOCK_STREAM, 0);  //cria um novo socket

  if (fd < 0) {
    printf("ERROR opening socket\n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;                //endereço da familia - sempre AF_INET
  serv_addr.sin_port = htons(port);              //Numero da porta - #DEFINE
  serv_addr.sin_addr.s_addr = INADDR_ANY;        //IP da maquina - INADDR_ANY faz isso por nos

  err = bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if ( err < 0) {
    printf("ERROR binding\n");
    exit(1);
  }

  listen(fd,5);

  int clilen = sizeof(cli_addr);
  newfd = accept(fd, (struct sockaddr *) &cli_addr, &clilen);
  if (newfd < 0) {
    printf("ERROR accepting socket\n");
    exit(1);
  }

  printf("Conected with success to socket: %d!\n", fd);

  return newfd;
}

int disconnect (int fd)
{
  printf("Disconected with success from socket: %d!\n\n\n", fd);
  return shutdown(fd, 2);
}

/*
Write_multiple_coils(fd, st_c, n_c, val)
{
// verifica consistência dos parâmetros
// constroi APDU
//(PDU de MODBUS) Send_Modbus_request (fd, APDU, APDU_R)
// analiza resposta
//(APDU_R ou código erro )
// retorna: num coils escritas
//– ok, <0
//– erro
}

Read_coils(fd, st_c, n_c, val)
{
// verifica consistência dos parâmetros
// constroi APDU
//(PDU de MODBUS) Send_Modbus_request (fd, APDU, APDU_R )
// analiza resposta
//(APDU_R ou código erro )
// retorna: num coils lidas
//– ok, <0
//– erro
}

Request_handler ( )
*/
