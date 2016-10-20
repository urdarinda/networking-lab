#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <time.h>
int main(int argc,char *argv[]){
	if(argc!=2)
	{
		printf("Usage (port)\n");
		return 0;
	}
	struct sockaddr_in addrport;
	
	
	int sockid=socket(PF_INET, SOCK_DGRAM, 0);
	if(sockid<0)
		{printf("Error In creating Socket\nPlease run again\n"); return 0;}

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(atoi(argv[1]));
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	int status= bind(sockid,(struct sockaddr *) &addrport, sizeof(addrport));
	
	
	int count;
	
	time_t cur_time;
	char * ct;
	struct sockaddr_in caddr;
	char recvbuf[20];
	int len=sizeof(caddr);
	recvfrom(sockid, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *) &caddr, &len);
	
	printf("Recieved : %s\n",recvbuf);
	cur_time=time(NULL);
	ct=ctime(&cur_time);
	sendto(sockid, ct, strlen(recvbuf), 0, (struct sockaddr *) &caddr, sizeof(caddr));
	

	return 0;
}
