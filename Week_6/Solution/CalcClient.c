#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
int main(int argc,char *argv[]){
if(argc!=3)
	{
		printf("Usage [hostname] [port]\n");
		return 0;
	}
int clientSocket, portNum, nBytes,sv;
char buffer[1024];
struct sockaddr_in serverAddr, clad;
socklen_t addr_size;

clientSocket = socket(PF_INET, SOCK_STREAM, 0);

portNum = atoi(argv[2]);

struct hostent *he;
struct in_addr **ad;
if ( (he = gethostbyname( argv[1] ) ) == NULL) 
{
    // get the host info
    herror("Error");
    return 1;
}
ad=(struct in_addr **) he->h_addr_list;
char ip[100];
strcpy(ip,inet_ntoa(*ad[0]));

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(portNum);
serverAddr.sin_addr.s_addr = inet_addr(ip);



addr_size = sizeof serverAddr;

if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0)
	{ 
		printf("connect error\n");
		return 0;
	}
else
	printf("Connected to %s:%d \n",ip,portNum);
printf("What do you want to calculate?\n Operator1 Operator(+ - * / ^) Operator2: ");
fgets(buffer,1024,stdin);
char a[50];
printf("You typed: %s",buffer);

nBytes = strlen(buffer) + 1;


send(clientSocket,buffer,nBytes,0);

recv(clientSocket, a, 1024, 0);

printf("Received from server: %s \n",a);   

close(clientSocket);
return 0;
}
