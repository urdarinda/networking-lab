#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc,char *argv[]){
	if(argc!=3)
	{
		printf("Usage (ip) (string)\n");
		return 0;
	}
  int clientSocket, portNum, nBytes;
 
  struct sockaddr_in serverAddr, clad;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  portNum = 7891;

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum);
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  
  clad.sin_family = AF_INET;
  clad.sin_port = htons(7892);
  clad.sin_addr.s_addr = htonl(INADDR_ANY);
  
  bind(clientSocket,(struct sockaddr *) &clad, sizeof(clad));

  addr_size = sizeof serverAddr;
  

  
    
    char a[50];
    printf("You typed: %s\n",argv[2]);

    nBytes = strlen(argv[2]) + 1;
    printf("Connecting to server ...\n");
	sendto(clientSocket, argv[2], nBytes, 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
 	printf("Connected to server ...\n");
	recvfrom(clientSocket, a,50, 0, (struct sockaddr *) &serverAddr, &addr_size);
    printf("Received from server: %s\n\n",a);   


  return 0;
}
