#include <stdio.h>
#include <stdlib.h>


#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <ctype.h>

#define port 6666

void stringToUpper(char buf[],int len);
int main(int argc, char *argv[]) {
  int s1;         //socket system call
  int news1;      //socket accept system
  int err, n;     //handling errors / n: read/write
  struct sockaddr_in serv_addr, cli_addr;  //endereços de cliente e servidor
  char buf[256];

/**********STEPS***************/

//1. Create a SOCKET point: Socket System Call
  //AF_INET: Usar AF_INET em todo o lado (embora correto correto seria aqui usar PF_INET e no sin_family usar AF_INET - i think)
  //SOCK_STREAM: tipo de socket- STREAM(lido seguido - TCP) vs DGRAM(chunks - UDP)
  //0: protocolo. Meter sempre 0 excepto raras exceções
  //return int or -1(fail - raro falhar)
  s1 = socket(AF_INET, SOCK_STREAM, 0);  //cria um novo socket

  if (s1 < 0) {
    perror("ERROR opening socket\n");
    exit(1);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr)); //inicializa o endereço do servidor como 0
  serv_addr.sin_family = AF_INET;                //endereço da familia - sempre AF_INET
  serv_addr.sin_port = htons(port);              //Numero da porta - #DEFINE
  serv_addr.sin_addr.s_addr = INADDR_ANY;        //IP da maquina - INADDR_ANY faz isso por nos

//2. BIND the message
  //Conecta um socket ao seu endereço e porta correspondentes em que o servidor esta a correr
  //s1: socket em questao
  //&serv_addr: endereço do servidor (pointer)
  //tamanho do endereço
  //pode falhar por diversos motivos. ex: socket já em uso nesta máquina
  err = bind(s1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if ( err < 0) {
    printf("ERROR binding\n");
    exit(1);
  }

//3. LISTEN: permite ao socket ouvir por conexões
  //s1: socket
  //numero de conexoes que podem estar a espera (5: maximo numero permitido na maioria das máquinas)
  //Nao checo por erros porque a unica maneira de falhar é se s1 nao for valido. Nesse caso, não chego aqui
  listen(s1,5);

//4. ACCEPT conexao
  //Faz blocking até  haver conexao com o cliente
  //retorna um novo socket (nao sei se é bem a terminologia correta) que devemos usar daqui em diante
  //2º e 3º argumento é um poiter para o endereço do cliente e seu tamanho
  //
  int clilen = sizeof(cli_addr);
  news1 = accept(s1, (struct sockaddr *) &cli_addr, &clilen);
  if (news1 < 0) {
    printf("ERROR accepting socket\n");
    exit(1);
  }

//5. Cicle of comms
  //So chegamos a este ponto se a conexao for bem sucedida
  //read: faz blocking ate ter algo para ler do cliente (depois do cliente ter executado um write) retorna numero de chars lidos
  //write: escreve para o cliente ler

  while (1)
  {
    bzero(buf,256);
    /* Now ask for a message from the user, this message
      * will be read by server
    */
    printf("Client sends: ");
    n = read(news1,buf,255);
    if ( !strcmp(buf, "#\n") )
    {
      write(news1,"Conection Closed! Have a good Day!\n",37);
      break;
    }

    printf("%s", buf);
    if ( n < 0)
      printf("ERROR - READ\n");

    stringToUpper(buf, n);
    n = write(news1,buf,n);
    if (n < 0)
      printf("ERROR - WRITE\n");
  }

  close(s1);
  close(news1);

  return 0;
}

void stringToUpper(char *buf,int len){
  for(int i = 0; i < len; i++)
  {
    buf[i] = toupper(buf[i]);
  }
}
