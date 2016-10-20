#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
int main(int argc,char *argv[]){
if(argc!=5|| ( (strcmp(argv[3],"PUT")!=0) && (strcmp(argv[3],"GET")!=0) ) )
	{
		printf("Usage [hostname] [port] [GET|PUT] [Filename]\nThis program can translate hostnames into ip\n");
		return 0;
	}
int fp ;
if(((fp=open(argv[4] ,O_RDONLY))<0) && (strcmp(argv[3],"PUT")==0) )
	{ 
		printf("File does not exist") ; 
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
		close(clientSocket);
		return 0;
	}
else
	printf("Connected to %s:%d \n",ip,portNum);


char a[102400+5];

if(strcmp(argv[3],"GET")==0)
{
		sprintf(buffer , "GET /%s HTTP/1.0 \r\n\r\n" , argv[4]) ;
		nBytes = strlen(buffer);
		send(clientSocket,buffer,nBytes,0);
		printf("Creating a request : %s",buffer);
		printf("Received from server:\n"); 
		while(recv(clientSocket, a, 102400, 0)>0) 
			printf("%s",a); 
		
}
else 
{

		sprintf(buffer , "PUT /%s \r\n" , argv[4]) ;
		nBytes = strlen(buffer);
		send(clientSocket,buffer,nBytes,0);
		printf("Creating a request : %s",buffer);
		char input[100000] ;
		

		
		char buffer[2000] ;
		
		while(read(fp , buffer , 1900)>0)
		{
			printf("\nSending  %s\n",buffer);
			send(clientSocket , buffer, strlen(buffer) , 0) ;
		}
		
		sprintf(buffer , "\r\n") ;
		
		send(clientSocket,buffer, strlen(buffer),0) ;

		printf("Received from server:\n"); 
		int rmessage = recv(clientSocket, input , 100000 , 0) ;
		printf("%s\n" , input)  ;
		
		close(fp) ;
}

close(clientSocket) ;

return 0;
}
