#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int bound_array[40];

void UDP_client(){
  int udpSocket, nBytes;
  //char buffer[1024];
  struct sockaddr_in serverAddr, clientAddr;
  //struct sockaddr_storage ;
  socklen_t addr_size, client_addr_size;
  int arrayToSend[40];
  int i;

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("192.168.21.136");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Bind socket with address struct*/
  //bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverAddr;

  printf("start sending parking space boundaries through UDP\n");

  while(1){
    //printf("start sending parking space boundaries \n");
    /* Try to receive any incoming UDP datagram. Address and port of 
      requesting client will be stored on serverStorage variable */
    //nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);

    /*Convert message received to uppercase*/
    // for(i=0;i<nBytes-1;i++)
    //   buffer[i] = toupper(buffer[i]);

    /*Send uppercase message back to client, using serverStorage as the address*/
  for (int i = 0 ; i < 40 ; ++i) {
    arrayToSend[i] = htonl(bound_array[i]) ;
    //printf("bound_array[%d] is : %d \n", i, bound_array[i]);
      }
    //sendto(udpSocket,arrayToSend,sizeof(arrayToSend),0,(struct sockaddr *)&serverAddr,addr_size);

  if(sendto(udpSocket,bound_array,sizeof(arrayToSend),0,(struct sockaddr *)&serverAddr,addr_size) == -1)
    printf("error \n");


  //return 0;
          }
}