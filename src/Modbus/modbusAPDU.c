#include "modbusAPDU.h"
#include "ModbusDataModel.h"

/**
 *
 */
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

/**
 *
 */
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

  int yes=1;

  // lose the pesky "Address already in use" error message
  if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }

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

/**
 *
 */
int disconnect (int fd)
{
  printf("\nDisconected with success from socket: %d!\n\n\n", fd);

  return close(fd);
}

/**
 *
 */
int Write_multiple_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  unsigned int N, i;
  unsigned char PDU[256], PDU_R[5];

  // Check Parameter consistency
  // start coil between 0x0000 and 0xFFFF
  if ( startCoilAddr < 0 || startCoilAddr > 0xFFFF ) {
    printf("Error with start Address");
    return -2;
  }
  //nCoils between 0x0001 and 0x07B0
  if ( nCoils < 0 || nCoils > 0x07B0 ) {
    printf("Error with number of coils");
    return -2;
  }

  // Create PDU
  if (nCoils % 8 != 0)
    N = nCoils / 8 + 1;
  else
    N = nCoils / 8;

  printf("N+6=%d\n", (N+6));
  print_hex("PDU_blank", PDU, (N+6));

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

  print_hex("PDU", PDU, N+6);

  // Create PDU_R
  //everything fine:  1 byte (0x0F) + 2 bytes (startCoilAddr) + 2 bytes (nCoils)
  //error:            1 byte (0x8F) + 1 byte (exception Code)

  //Send Request
  int res = Send_Modbus_request(fd, PDU, PDU_R, N+6);

  // check response
  if ( res == -1) {
    printf("Error sending Modbus Request - timeout");
    print_hex("PDU_R", PDU_R, 2);
    return -1;
  }

  if ( PDU_R[0] == 0x8F ) {
    printf("Error sending Modbus Request - Error: %x", PDU_R[1]);
      print_hex("PDU_R", PDU_R, 2);
    return -1;
  }

  print_hex("PDU_R", PDU_R, 5);

  //No error:
  return nCoils;
}

/**
 *
 */
int Read_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
  //unsigned int N;
  unsigned char PDU[5], PDU_R[260];

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

  /* Create PDU
  if (nCoils % 8 != 0)
    N = nCoils / 8 + 1;
  else
    N = nCoils / 8;
 */
  // Create PDU:

  // Function Code: 0x01 - Read
  PDU[0] = 0x01;
  // start Address
  PDU[1] = (startCoilAddr >> 8) & 0xff;
  PDU[2] = (startCoilAddr & 0xff);
  // Qty of outpus
  PDU[3] = (nCoils >> 8) & 0xff;
  PDU[4] = nCoils & 0xff;

  // Create PDU_R
  //everything fine:  1 byte (0x01) + 1 bytes (N ou N+1) + N bytes (valores)
  //error:            1 byte (0x01 + 0x8F)???? + 1 byte (exception Code)
  //PDU_R = (unsigned char*)malloc(2 * sizeof(unsigned char));

 //Send Request
  int res = Send_Modbus_request(fd, PDU, PDU_R, 5);

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

/**
 *
 */
int Request_handler (int fd)
{
  int TI, PDU_Rsize;
  unsigned char PDU_P[256], *PDU_R;

  // Recebe um pedido
  int PDU_Psize = Receive_Modbus_request(fd, PDU_P, &TI);
  print_hex("PDU_P", PDU_P , PDU_Psize);

  // analiza e executa pedido se correto
  if(PDU_P[0] == 0x0F) {
      int startCoilAddr, nCoils, remainingBytes;

      printf("\nWrite multiple coils\n");

      // Decompoe PDU
      startCoilAddr =  (int)(PDU_P[1] << 8) + (int)(PDU_P[2]);
      nCoils =         (int)(PDU_P[3] << 8) + (int)(PDU_P[4]);
      remainingBytes = (int)(PDU_P[5]);

      unsigned char * valueCoils = (unsigned char*)malloc(remainingBytes * sizeof(unsigned char));

      for(int i = 0; i < remainingBytes; i++) {
          valueCoils[i] = PDU_P[i + 6];
      }

      int ok = W_coils(startCoilAddr, nCoils, valueCoils);

      PDU_R = (unsigned char*)malloc(5 * sizeof(unsigned char));
      if(ok == nCoils)
      {
        PDU_Rsize = 5;

        print_hex("Write-PDU_P", PDU_P, PDU_Psize);

        for(int i = 0; i < 5; i++)
            PDU_R[i] = PDU_P[i];

        print_hex("Write-PDU_R", PDU_R, 5);
      }
      else {
        //erro
        PDU_Rsize = 2;

        PDU_R[0] = 0x8F;
        PDU_R[1] = 0x69;
      }
  } else if(PDU_P[0] == 0x01) {
      int startCoilAddr, nCoils;
      unsigned char valueCoils[256];

      printf("\nRead coils\n");

      startCoilAddr = (int)(PDU_P[1] << 8) + (int)(PDU_P[2]);

      nCoils = (int)(PDU_P[3] << 8) + (int)(PDU_P[4]);

      int n = R_coils(startCoilAddr, nCoils, valueCoils);

      PDU_R = (unsigned char*)malloc((2 + n) * sizeof(unsigned char));
      PDU_Rsize = (2 + n);

      printf("\nRead coils2\n");

      if(n == nCoils) {
          int N;

          if (nCoils % 8 != 0)
            N = nCoils / 8 + 1;
          else
            N = nCoils / 8;

        PDU_R[0] = PDU_P[0];
        PDU_R[1] = N;

        for(int i = 0; i < N; i++)
          PDU_R[2 + i] = valueCoils[i];
      }

      printf("\nRead coils end\n");
  } else {
      printf("Received a function not supported.\n");
  }

  Send_Modbus_response(fd, PDU_R, TI, PDU_Rsize);

  // retorna: >0 – ok, <0 – erro
  return 1;
}
