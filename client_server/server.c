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
void* clientThread(void* vargp);
int main(int argc,char* argv[]){

	if(argc!=2){
		fprintf(stderr,"Usage: %s <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int listenfd,*connfd,clientlen;
	int optval=1;
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	pthread_t tid;
	char hbuf[NI_MAXHOST];

	if( (listenfd=socket(AF_INET,SOCK_STREAM,0))<0)	{
		fprintf(stderr,"Error: socket failed\n");
		exit(EXIT_FAILURE);
	}
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,
				(const void*)&optval,sizeof(int))<0){
		fprintf(stderr,"Error: setsockopt failed\n");
		exit(EXIT_FAILURE);
	}
	int port=atoi(argv[1]);
	printf("port=%d\n",port);
	bzero((char*)&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons((unsigned short)port);
	if(bind(listenfd,(SA*)&serveraddr,sizeof(serveraddr))<0){
		fprintf(stderr,"Error: bind failed\n");
		exit(EXIT_FAILURE);
	}
	if(listen(listenfd,LISTENQ)<0){
		fprintf(stderr,"Error: listen failed\n");
		exit(EXIT_FAILURE);
	}
	while(1){
		clientlen=sizeof(clientaddr);
		connfd=malloc(sizeof(int));
		*connfd=accept(listenfd,(SA*)&clientaddr,&clientlen);
		pthread_create(&tid,NULL,clientThread,connfd);
		getnameinfo((SA*)&clientaddr,clientlen,hbuf,
				sizeof(hbuf),NULL,0,NI_NAMEREQD);
		char* haddrp=inet_ntoa(clientaddr.sin_addr);/*thread
							     unsafe*/
		printf("server connected to %s (%s)\n",
				hbuf,haddrp);
	}
	exit(EXIT_SUCCESS);
}
void* clientThread(void* varfgp){
	pthread_detach(pthread_self());
	unsigned long tid=(unsigned long)pthread_self();

	int connfd=*((int*)varfgp);
	free(varfgp);

	ssize_t r_num;
	char buf[MAXLINE];
	
	while(1){
		r_num=read(connfd,buf,MAXLINE);
		if(r_num<0 || (strcmp(buf,"quit")==0))
			break;
		else if(r_num>0){
			if(strcmp(buf,"quit")==0)
				printf("quit ok ");
			printf("thread %lu received %s \n",tid,buf);
			buf[0]='\0';
		}
	}
	printf("thread %lu quit\n",tid);
	close(connfd);
	return NULL;
}
