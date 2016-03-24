#include "ApplicationLayer.h"

int connectClient (char* ip_serv, int port)
{
  int fd, err;
  struct sockaddr_in serv_addr;           // server address
  struct in_addr addr;                    // client address

  fd = socket(AF_INET, SOCK_STREAM, 0);   // create new socket

  if (fd < 0) {
    printf("ERROR opening socket\n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;         // Family address - always AF_INET
  serv_addr.sin_port = htons(port);       // Port number - #DEFINE
  inet_aton(ip_serv, &addr);              // machine IP - INADDR_ANY(insted of ip_serv(127.0.0.1) does some magic
  serv_addr.sin_addr = addr;

  err = connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); // connect

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
  struct sockaddr_in serv_addr, cli_addr;       // server and client addresses

  fd = socket(AF_INET, SOCK_STREAM, 0);         // create new server socket

  if (fd < 0) {
    printf("ERROR opening socket\n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;                // family addr - always AF_INET
  serv_addr.sin_port = htons(port);              // Port number - #DEFINE
  serv_addr.sin_addr.s_addr = INADDR_ANY;        // machine IP - INADDR_ANY does magic

  int yes = 1;

  // lose the pesky "Address already in use" error message
  if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  // assigns a socket (fd) to an address
  err = bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if ( err < 0) {
    printf("ERROR binding\n");
    exit(1);
  }

  // prepares it for incoming connections
  listen(fd,5);

  socklen_t clilen = sizeof(cli_addr);  //int???

  // creates a new socket for each connection and removes the connection from the listen queue
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
  printf("\nDisconected with success from socket: %d!\n\n\n", fd);

  return close(fd);
}

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
  // nCoils between 0x0001 and 0x07B0
  if ( nCoils <= 0 || nCoils > 0x07B0 ) {
    printf("Error with number of coils");
    return -2;
  }

  // Create PDU
  if (nCoils % 8 != 0)
    N = nCoils / 8 + 1;
  else
    N = nCoils / 8;

  // print_hex("PDU_blank", PDU, (N+6));   // for DEBUG

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

  // print_hex("PDU", PDU, N+6);      // for DEBUG

  // Create PDU_R
  //everything fine:  1 byte (0x0F) + 2 bytes (startCoilAddr) + 2 bytes (nCoils)
  //error:            1 byte (0x8F) + 1 byte (exception Code)

  // Send Request
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

  // print_hex("PDU_R", PDU_R, 5);    // for DEBUG

  //No error:
  return nCoils;
}

int Read_coils(int fd, int startCoilAddr, int nCoils, unsigned char* valueCoils)
{
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

  for(int i=0; i < PDU_R[1]; i++)
    valueCoils[i] = PDU_R[i+2];

  //No error:
  return nCoils;
}

int Request_handler (int fd)
{
  int TI, PDU_Rsize;
  unsigned char PDU_P[256], *PDU_R;

  // receive request
  int PDU_Psize = Receive_Modbus_request(fd, PDU_P, &TI);
  // print_hex("PDU_P", PDU_P , PDU_Psize);             // for DEBUG

  if ( PDU_Psize <= 0)
  {
    printf("Error receiving PDU");
    return -1;
  }

  // check and execute request ( if no errors )
  // write multiple coils
  if(PDU_P[0] == 0x0F) {
      int startCoilAddr, nCoils, remainingBytes;

      printf("\nWrite multiple coils\n");

      // Decompose PDU into startCoilAddr, nCoils and number of remainingBytes (Data -> valueCoils)
      startCoilAddr =  (int)(PDU_P[1] << 8) + (int)(PDU_P[2]);
      nCoils =         (int)(PDU_P[3] << 8) + (int)(PDU_P[4]);
      remainingBytes = (int)(PDU_P[5]);

      unsigned char * valueCoils = (unsigned char*)malloc(remainingBytes * sizeof(unsigned char));

      for(int i = 0; i < remainingBytes; i++) {
          valueCoils[i] = PDU_P[i + 6];
      }

      // write coils into Driver
      int ok = W_coils(startCoilAddr, nCoils, valueCoils);

      PDU_R = (unsigned char*)malloc(5 * sizeof(unsigned char));

      //everything ok
      if(ok == nCoils)
      {
        PDU_Rsize = 5;
        // print_hex("Write-PDU_P", PDU_P, PDU_Psize);        // for DEBUG
        for(int i = 0; i < 5; i++)
            PDU_R[i] = PDU_P[i];
        // print_hex("Write-PDU_R", PDU_R, 5);                   // for DEBUG
      }
      // error
      else {
        PDU_Rsize = 2;

        PDU_R[0] = 0x8F;
        PDU_R[1] = 0x69;
      }
  }
  // read multiple coils
  else if(PDU_P[0] == 0x01) {
      int startCoilAddr, nCoils;
      unsigned char valueCoils[256];

      printf("\nRead coils\n");

      // Decompose PDU into startCoilAddr and nCoils
      startCoilAddr = (int)(PDU_P[1] << 8) + (int)(PDU_P[2]);
      nCoils = (int)(PDU_P[3] << 8) + (int)(PDU_P[4]);

      // Read Coils from Driver
      int n = R_coils(startCoilAddr, nCoils, valueCoils);

      if(n == nCoils) {
          int N;

          if (nCoils % 8 != 0)
            N = nCoils / 8 + 1;
          else
            N = nCoils / 8;

        PDU_Rsize = N + 2;
        PDU_R = (unsigned char*)malloc((2 + N) * sizeof(unsigned char));

        PDU_R[0] = PDU_P[0];
        PDU_R[1] = N;

        for(int i = 0; i < N; i++)
          PDU_R[2 + i] = valueCoils[i];
      }

      printf("\nRead coils end\n");
  }
  // error (function code not recognized)
  else {
      printf("Received a function not supported.\n");
  }

  Send_Modbus_response(fd, PDU_R, TI, PDU_Rsize);

  // return: >0 – ok, <0 – error
  return 1;
}
