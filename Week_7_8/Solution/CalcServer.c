#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
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
struct targs{
	int cl;
	int id;
};
void *tmain(void* arg)
{
	int s;
	pthread_detach(pthread_self());
	s=((struct targs*) arg)->cl;
	//
	
		char a[20]="";
	
		if(recv(s,a,20, 0)<0) printf("Recv error");
		printf("Client %d sent : %s ",((struct targs*) arg)->id,a);
		
		double d=atof(strtok(a," "));
		char c=(char)strtok(NULL," ");
		double b=atof(strtok(NULL," "));
		
		for(int i=0;i<20;i++)
		{
			if(a[i]=='+' || a[i]=='-' || a[i]=='*' || a[i]=='/' || a[i]=='^') c=a[i];
		}
		

		char ans[10];
		sprintf(ans,"%0.3lf",calc(d,b,c));
		printf("Ans [%d] = %s \n",((struct targs*) arg)->id,ans);
		send(s,ans,10,0);
		close(s);
		free(arg);

}
int main(int argc,char *argv[]){
	
	pthread_t tid;
	struct targs *targ;



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
	
	
	int count,co=0;
	
	while(1)
	{
		char str[20];
		unsigned int lenc=sizeof(caddr);
		int s=accept(sockid,(struct sockaddr *) &caddr,&lenc);
		inet_ntop(AF_INET, &(caddr.sin_addr), str, lenc);
		printf("Client [%d] %s connected on port %d\n",++co,str,port);
		if((targ=(struct targs *)malloc(sizeof(struct targs)))==NULL)
		{
			printf("Not enough memory\n");
			exit(0);
		}
		targ->cl=s;
		targ->id=co;
		if(pthread_create(&tid,NULL,tmain,(void*)targ)!=0)
		{
			printf("Cant create thread");
			exit(0);
		}

		
		
	}
	
	close(sockid);
	return 0;
}
