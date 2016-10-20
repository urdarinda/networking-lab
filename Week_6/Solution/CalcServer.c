#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <math.h>
double calc(double a,double b,char c)
{
	switch(c)
	{
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;
		case '^': return pow(a,b);
		default: return 0;
	}
}
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
	
	
	int count;
	
	while(1)
	{
		char str[20];
		unsigned int lenc=sizeof(caddr);
		int s=accept(sockid,(struct sockaddr *) &caddr,&lenc);
		
		inet_ntop(AF_INET, &(caddr.sin_addr), str, lenc);
		printf("Client %s connected on port %d\n",str,port);
		char a[20]="";
	
		recv(s,a,20, 0);
		printf("Client sent : %s \n",a);
		
		double d=atof(strtok(a," "));
		char c=(char)strtok(NULL," ");
		double b=atof(strtok(NULL," "));
		
		for(int i=0;i<20;i++)
		{
			if(a[i]=='+' || a[i]=='-' || a[i]=='*' || a[i]=='/' || a[i]=='^') c=a[i];
		}
		

		char ans[10];
		sprintf(ans,"%0.3lf",calc(d,b,c));
		printf("Ans =%s \n\n",ans);
		send(s,ans,10,0);
		close(s);
	}
	
	close(sockid);
	return 0;
}
