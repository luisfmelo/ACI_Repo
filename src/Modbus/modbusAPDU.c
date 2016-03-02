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

  socklen_t clilen = sizeof(cli_addr);  //int???
  newfd = accept(fd, (struct sockaddr *) &cli_addr, &clilen); //---------------->&clilen??
  if (newfd < 0) {
    printf("ERROR accepting socket\n");
    exit(1);
  }

  printf("Conected with success to socket: %d!\n", fd);

  return newfd;
}

int disconnect (int fd)
{
  printf("\nDisconected with success from socket: %d!\n\n\n", fd);
  return shutdown(fd, 2);
}

int Write_multiple_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  unsigned int N, i;
  unsigned char *PDU, *PDU_R;

 // Check Parameter consistency
  // start coil between 0x0000 and 0xFFFF
  if ( startCoilAddr < 0 || startCoilAddr > 0xFFFF )
  {
    printf("Error with start Address");
    return -2;
  }
  //nCoils between 0x0001 and 0x07B0
  if ( nCoils < 0 || nCoils > 0x07B0 )
  {
    printf("Error with number of coils");
    return -2;
  }

  // Create PDU
  if (nCoils % 8 != 0)
    N = nCoils / 8 + 1;
  else
    N = nCoils / 8;

  // Create PDU:
  PDU = (unsigned char*)malloc((N + 6) * sizeof(unsigned char));

  // Function Code: 0x0F
  PDU[0] = 0x0F;
  // start Address
  PDU[1] = (startCoilAddr >> 8) & 0xff;
  PDU[2] = (startCoilAddr & 0xff);
  // Qty of outpus
  PDU[3] = (nCoils >> 8) & 0xff;
  PDU[4] = nCoils & 0xff;
  // Byte Count
  PDU[5] = N;

  // Data
  for (i = 0; i < N; i++)
    PDU[i + 6] = valueCoils[i];

 // Create PDU_R
  //everything fine:  1 byte (0x0F) + 2 bytes (startCoilAddr) + 2 bytes (nCoils)
  //error:            1 byte (0x8F) + 1 byte (exception Code)
  PDU_R = (unsigned char*)malloc(5 * sizeof(unsigned char));

 //Send Request
  int res = Send_Modbus_request (fd, PDU, PDU_R);

 // check response
  if ( res == -1)
  {
    printf("Error sending Modbus Request - timeout");
    return -1;
  }
  //ou else if? se for timeout da sempre erro?
  if ( PDU_R[0] == 0x8F )
  {
    printf("Error sending Modbus Request - Error: %c", PDU_R[1]);
    return -1;
  }

  //No error:
  return nCoils;
}

int Read_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  unsigned int N, i;
  unsigned char *PDU, *PDU_R;

 // Check Parameter consistency
  // start coil between 0x0000 and 0xFFFF
  if ( startCoilAddr < 0 || startCoilAddr > 0xFFFF )
  {
    printf("Error with start Address");
    return -2;
  }
  //nCoils between 0x0001 and 0x07B0
  if ( nCoils < 0 || nCoils > 0xFFFF )
  {
    printf("Error with number of coils");
    return -2;
  }

  // Create PDU
  if (nCoils % 8 != 0)
    N = nCoils / 8 + 1;
  else
    N = nCoils / 8;

  // Create PDU:
  PDU = (unsigned char*)malloc((N + 6) * sizeof(unsigned char));

  // Function Code: 0x01
  PDU[0] = 0x01;
  // start Address
  PDU[1] = (startCoilAddr >> 8) & 0xff; //MSB
  PDU[2] = startCoilAddr & 0xff;        //LSB
  // Qty of outpus
  PDU[3] = (nCoils >> 8) & 0xff;        //MSB
  PDU[4] = nCoils & 0xff;               //LSB
  // Byte Count
  PDU[5] = N;

  // Data
  for (i = 0; i < N; i++)
    PDU[i + 6] = valueCoils[i];

 // Create PDU_R
  //everything fine:  1 byte (0x0F) + 2 bytes (startCoilAddr) + 2 bytes (nCoils)
  //error:            1 byte (0x8F) + 1 byte (exception Code)
  PDU_R = (unsigned char*)malloc(5 * sizeof(unsigned char));

 //Send Request
  int res = Send_Modbus_request (fd, PDU, PDU_R);

 // check response
  if ( res == -1)
  {
    printf("Error sending Modbus Request - timeout");
    return -1;
  }
  //ou else if? se for timeout da sempre erro?
  if ( PDU_R[0] == 0x8F )
  {
    printf("Error sending Modbus Request - Error: %c", PDU_R[1]);
    return -1;
  }

  //No error:
  return nCoils;
}

int Request_handler (int fd)
{
  //Receive_Modbus_request (fd, APDU_P, TI)
  // analiza e executa pedido se correto
  //W_coils (st_c, n_c, val) ou R_coils (st_c, n_c, val)
  // prepara e envia APDU de resposta
  //Send_Modbus_response (fd, APDU_R, TI)
  // retorna: >0 – ok, <0 – erro
  return 1;
}
