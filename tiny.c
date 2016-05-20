/*
 *Describe: tiny.c
 *	A simple ,multi-threads web server that uses 
 *	the GET method to server static and dynamic content.
 */
#include "lib/csapp.h"
#include "transaction.h"
void* clientThread(void* vargp);

void command(void){
	char buf[MAXLINE];
	if(!Fgets(buf,MAXLINE,stdin)){
		exit(EXIT_SUCCESS);/*EOF*/
	}
	printf("%s",buf);/*Process the input command*/
}
int main(int argc,char* argv[]){

	if(argc!=2){
		fprintf(stderr,"Usage: %s <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int listenfd,*connfdp,clientlen;
	struct sockaddr_in clientaddr;
	pthread_t tid;
	//struct hostent *hp;
	char hbuf[NI_MAXHOST];
	listenfd=Open_listenfd(argv[1]);

	while(1){
		clientlen=sizeof(clientaddr);
		hbuf[0]='\0';
		connfdp=Malloc(sizeof(int));
		*connfdp=Accept(listenfd,(SA*)&clientaddr,&clientlen);
		/*Create new thread*/
		Pthread_create(&tid,NULL,clientThread,connfdp);

		Getnameinfo((SA*)&clientaddr,clientlen,hbuf,
				sizeof(hbuf),NULL,0,NI_NAMEREQD);
		char* haddrp=inet_ntoa(clientaddr.sin_addr);
		printf("server connected to %s (%s)\n",
				hbuf,haddrp);

	}
	exit(EXIT_SUCCESS);
}
void* clientThread(void* varfgp){
	int connfd=*((int*)varfgp);
	Pthread_detach(pthread_self());
	Free(varfgp);
	
	doit(connfd);
	Close(connfd);
	return NULL;
}
