/*
 *Describe: tiny.c
 *	A simple ,mutli-process HTTP/1.0 web server that uses 
 *	the GET method to server static and dynamic content.
 */
//#include "../lib/csapp.h"
#include "lib/csapp.h"
#include "transaction.h"
static void signalchild_handler(int sig){
	while(waitpid(-1,0,WNOHANG)>0){
		;
	}
	return;
}
int main(int argc,char* argv[]){
	
	if(argc!=2){
		fprintf(stderr,"Usage: %s <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	/*create signal handler*/
	Signal(SIGCHLD,signalchild_handler);

	int listenfd,connfd,port,clientlen;
	struct sockaddr_in clientaddr;
	//struct hostent *hp;
	char hbuf[NI_MAXHOST];
	listenfd=Open_listenfd(argv[1]);
	while(1){
		clientlen=sizeof(clientaddr);
		hbuf[0]='\0';
		connfd=Accept(listenfd,(SA*)&clientaddr,&clientlen);
		/*
		hp=Gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr
			,sizeof(clientaddr.sin_addr.s_addr),AF_INET);
		*/
		/*the Gethostbyaddr not thread safty*/
		Getnameinfo((SA*)&clientaddr,clientlen,hbuf,
				sizeof(hbuf),NULL,0,NI_NAMEREQD);
		char* haddrp=inet_ntoa(clientaddr.sin_addr);
		printf("server connected to %s (%s)\n",
				hbuf,haddrp);
		
		/*
		if(hp!=NULL)
			printf("server connected to %s (%s)\n",hp->h_name,haddrp);
		else
			printf("server connected to unknow(%s)\n",
					haddrp);
		*/
		/*create new process*/
		if(Fork()==0){
			Close(listenfd);
			doit(connfd);/* Note: parent and child share
				      the file descriptor,but address
				      space not.*/
			Close(connfd);
			exit(EXIT_SUCCESS);
		}
		Close(connfd); /* Parent closes connected socket(impormant)*/
	}

	exit(EXIT_SUCCESS);
}
