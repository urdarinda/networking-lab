#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc,char *argv[]){
  if(argc!=3)
		{printf("Usage [ip] [port]\n");return 0;}
  int clientSocket, portNum, nBytes,sv;
  char buffer[1024];
  struct sockaddr_in serverAddr, clad;
  socklen_t addr_size;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  portNum = atoi(argv[2]);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum);
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  
  

  addr_size = sizeof serverAddr;
 
	if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0) printf("connect error\n");
 	else
 	{
	    printf("Type a sentence to send to server: ");
	    fgets(buffer,1024,stdin);
	    char a[50];
	    printf("You typed: %s",buffer);

	    nBytes = strlen(buffer) + 1;


	    send(clientSocket,buffer,nBytes,0);

	    recv(clientSocket, a, 1024, 0);

	    printf("Received from server: %s\n\n",a);   
	}
	close(clientSocket);
  return 0;
}
