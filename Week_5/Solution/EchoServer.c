#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(){

	struct sockaddr_in addrport;
	
	
	int sockid=socket(PF_INET, SOCK_DGRAM, 0);
	if(sockid<0)
		{printf("Error In creating Socket\nPlease run again\n"); return 0;}

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(7891);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	int status= bind(sockid,(struct sockaddr *) &addrport, sizeof(addrport));
	
	
	int count;
	
	
	struct sockaddr_in caddr;
	char recvbuf[20];
	int len=sizeof(caddr);
	recvfrom(sockid, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *) &caddr, &len);
	
	printf("Recieved : %s\n",recvbuf);
	sendto(sockid, recvbuf, strlen(recvbuf), 0, (struct sockaddr *) &caddr, sizeof(caddr));
	

	return 0;
}
