#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char *argv[]){

	struct sockaddr_in addrport;struct sockaddr_in caddr;
	if(argc!=2)
		{printf("Usage [port]\n");return 0;}
	
	int sockid=socket(PF_INET, SOCK_STREAM, 0);
	if(sockid<0)
		{printf("Error In creating Socket\nPlease run again\n"); return 0;}
	int port=atoi(argv[1]);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(port);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	int status= bind(sockid,(struct sockaddr *) &addrport, sizeof(addrport));
	
	if(listen(sockid,5)<0) printf("listen error\n");
	printf("Server listening on port %d..  \n",port);
	
	
	int count; int co=0;
	
	while(1)
	{
		char str[20];
		unsigned int lenc=sizeof(caddr);
		int s=accept(sockid,(struct sockaddr *) &caddr,&lenc);
		
		inet_ntop(AF_INET, &(caddr.sin_addr), str, lenc);
		printf("Client [%d] %s connected on port %d\n",++co,str,port);
		if(fork()==0)
		{
			char recvbuf[20]="";
			char rev[20];
	
			recv(s,recvbuf,20, 0);
			printf("Client %d sent : %s \n",co,recvbuf);
			int i=0,j;
			for(i=strlen(recvbuf)-1,j=0;i>=0;i--) rev[j++]=recvbuf[i];
			rev[j]='\0';	
			
			send(s,rev,sizeof(rev),0);
			close(s);
			exit(0);
		}
		
		close(s);
	}
	
	close(sockid);
	return 0;
}
