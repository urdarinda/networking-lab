#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
void sigp(int s)
{
	//if(s==SIGINT)
	//{ 

		printf("Recieved SIGINT....\n Closing all sockets and exiting normally\n");
		exit(0);


	//}
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
	char a[20]="";
	if(recv(s,a,4, 0)<0) 
			{
				printf("Recv error\n");
				return 0;
			}
	

	if(strcmp(a,"GET ")==0) 
	{
		if(recv(s,a,20, 0)<0) 
			{
				printf("Recv error\n");
				return 0;
			}
		char filename[20] ;
		int k=0;
		for(int i=1 ; a[i]!=' ' ; i++) 
			filename[k++] = a[i] ;
		
		filename[k]='\0' ;
		printf("Client %d sent : %s \n",((struct targs*) arg)->id,filename);
		if(access(filename, F_OK)==-1) 
		{
			char *reply="404 NOT FOUND\n" ;
			send(s , reply , strlen(reply) , 0) ;
			
		}
		else
		{
			char *reply="200 OK\n" ;
			send(s , reply , strlen(reply) , 0) ;
			char buffer[2000];
			int fp = open(filename ,O_RDONLY) ;
			
			int rlen = 0 ;
			
			while(rlen=read(fp , buffer , 1900)>0)
			{
				printf("\nSending  %s\n",buffer);
				send(s , buffer, strlen(buffer) , 0) ;
			}
			
			
			close(fp) ;
			
		}
	}
	else
	{
		if(recv(s,a,20, 0)<0) 
			{
				printf("Recv error\n");
				return 0;
			}
		char filename[20] ;
		int k=0;
		for(int i=1 ; a[i]!=' ' ; i++) 
			filename[k++] = a[i] ;
		
		filename[k]='\0' ;
		printf("Client %d sent : %s \n",((struct targs*) arg)->id,filename);
		char buffer[2000];
		
		k+=10 ;
		int fp;
		if((fp=open(filename ,O_WRONLY | O_CREAT , 0644))<0)
			{ 
				printf("File write error \n") ; 
				char *reply="400 FILE WRITE ERROR" ;
				send(s , reply , strlen(reply) , 0) ;
				return 0;
			}
			
		while(recv(s,buffer,1000, 0)>0)
		{
			//printf("%s\n",buffer);
			write(fp,buffer,strlen(buffer));
			if(buffer[strlen(buffer)-1]=='\n'&& buffer[strlen(buffer)-2]=='\r') break;
		}
		
		close(fp) ;
		
		char *reply="200 OK FILE CREATED" ;
		send(s , reply , strlen(reply) , 0) ;
		printf("created for %d\n",((struct targs*) arg)->id);
		
	}
		close(s);
		free(arg);





}
int main(int argc,char *argv[]){
	
	pthread_t tid;
	struct targs *targ;
	signal(SIGINT,sigp);
	while(1);


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
