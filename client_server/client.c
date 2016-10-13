/*
 *Describe: mimiqqserver.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define LISTENQ 1024
#define MAXLINE 4096
typedef struct sockaddr SA;
int open_clientfd(char* hostname,int port){
	int clientfd;
	struct hostent *hp;
	struct in_addr addr;
	struct sockaddr_in serveraddr;
	
	if( (clientfd=socket(AF_INET,SOCK_STREAM,0))<0){
		fprintf(stderr,"Error: socket failed\n");
		return -1;
	}
	if(inet_aton(hostname,&addr)!=0) /* dotted-decimal*/
		hp=gethostbyaddr((const char*)&addr,sizeof(addr),AF_INET);
	else
		hp=gethostbyname(hostname);
	if(hp==NULL){
		fprintf(stderr,"Error :host invailed\n");
		return -1;
	}
	bzero((char*)&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	bcopy((char*)hp->h_addr_list[0],
			(char*)&serveraddr.sin_addr.s_addr,hp->h_length);
	serveraddr.sin_port=htons(port);

	if(connect(clientfd,(SA*)&serveraddr,sizeof(serveraddr))<0){
		return -1;
	}
	return clientfd;
}
int main(int argc,char* argv[]){

	if(argc!=3){
		fprintf(stderr,"Usage: %s <host> <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	int clientfd,port;
	char *host,buf[MAXLINE];
	host=argv[1];
	port=atoi(argv[2]);

	if((clientfd=open_clientfd(host,port))<0){
		exit(EXIT_FAILURE);	
	}

	while(fgets(buf,MAXLINE,stdin)!=NULL){
		ssize_t w_num=write(clientfd,buf,strlen(buf));
		sprintf(buf,"   >>send %d bytes\n",w_num);
		fputs(buf,stdout);
		if(strcmp(buf,"quit")==0)
			break;
		buf[0]='\0';
		fflush(stdin);
		fflush(stdout);
	}
	printf("quit done.\n");
	close(clientfd);
	exit(EXIT_SUCCESS);
}
