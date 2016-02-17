#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#define port 6666

int main(int argc, char *argv[]) {
   int s1;         //socket
   int err, n;     //handling errors / n: read/write
   struct sockaddr_in serv_addr;  //endereço do servidor
   char buf[256];

 //1. Create a SOCKET point: Socket System Call
   //AF_INET: Usar AF_INET em todo o lado (embora correto correto seria aqui usar PF_INET e no sin_family usar AF_INET - i think)
   //SOCK_STREAM: tipo de socket- STREAM(lido seguido - TCP) vs DGRAM(chunks - UDP)
   //0: protocolo. Meter sempre 0 excepto raras exceções
   //return int or -1(fail - raro falhar)
   s1 = socket(AF_INET, SOCK_STREAM, 0);  //cria um novo socket

   if (s1 < 0) {
     printf("ERROR opening socket\n");
     exit(1);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr)); //inicializa o endereço do servidor como 0
   serv_addr.sin_family = AF_INET;                //endereço da familia - sempre AF_INET
   serv_addr.sin_port = htons(port);              //Numero da porta - #DEFINE
   serv_addr.sin_addr.s_addr = INADDR_ANY;        //IP da maquina - INADDR_ANY faz isso por nos

//2. CONNECT
  //Conecta um socket ao seu endereço e porta correspondentes em que o servidor esta a correr
  //s1: socket em questao
  //&serv_addr: endereço do servidor (pointer)
  //tamanho do endereço
  //pode falhar por diversos motivos. ex: socket já em uso nesta máquina
  err = connect(s1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if (err < 0) {
    printf("ERROR connecting\n");
    exit(1);
  }

  //5. Cicle of comms
    //So chegamos a este ponto se a conexao for bem sucedida
    //read: faz blocking ate ter algo para ler do cliente (depois do cliente ter executado um write) retorna numero de chars lidos
    //write: escreve para o cliente ler

   printf("Please enter the message: ");
   bzero(buf,256);
   fgets(buf,255,stdin);

   /* Send message to the server */
   if (write(s1, buf, strlen(buf)) < 0) {
      perror("ERROR writing to socket\n");
      exit(1);
   }

   /* Now read server response */
   bzero(buf,256);
   n = read(s1, buf, 255);

   printf("Server: %s\n",buf);

   close(s1);
   return 0;
}
