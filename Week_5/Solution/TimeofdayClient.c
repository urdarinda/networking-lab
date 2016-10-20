#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h> 
int main(int argc,char *argv[]){
	if(argc!=3)
	{
		printf("Usage (ip) (port)\n");
		return 0;
	}
  int clientSocket,nBytes;
 
  struct sockaddr_in serverAddr, clad;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[2]));
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  
  clad.sin_family = AF_INET;
  clad.sin_port = htons(7892);
  clad.sin_addr.s_addr = htonl(INADDR_ANY);
  
  bind(clientSocket,(struct sockaddr *) &clad, sizeof(clad));

  addr_size = sizeof serverAddr;
  

  
    
  char a[100]="Time please";
  nBytes = strlen(a) + 1;
  printf("Connecting to server ...\n");
	sendto(clientSocket, a, nBytes, 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
 	printf("Connected to server ...\n");
	recvfrom(clientSocket, a,100, 0, (struct sockaddr *) &serverAddr, &addr_size);
  printf("Received from server: %s\n\n",a);   


  return 0;
}
