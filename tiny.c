/*
 *Describe: tiny.c
 *	A simple ,I/O multiplexing Iteration web server that uses 
 *	the GET method to server static and dynamic content.
 */
//#include "../lib/csapp.h"
#include "lib/csapp.h"
#include "transaction.h"

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

	int listenfd,connfd,port,clientlen;
	struct sockaddr_in clientaddr;
	fd_set read_set,ready_set;

	//struct hostent *hp;
	char hbuf[NI_MAXHOST];
	listenfd=Open_listenfd(argv[1]);

	FD_ZERO(&read_set);
	FD_SET(STDIN_FILENO,&read_set);
	FD_SET(listenfd,&read_set);

	while(1){

		ready_set=read_set;
		Select(listenfd+1,&ready_set,NULL,NULL,NULL);/*Stall*/
		if(FD_ISSET(STDIN_FILENO,&ready_set))
			command();
		if(FD_ISSET(listenfd,&ready_set)){
			clientlen=sizeof(clientaddr);
			hbuf[0]='\0';
			connfd=Accept(listenfd,(SA*)&clientaddr,&clientlen);
			Getnameinfo((SA*)&clientaddr,clientlen,hbuf,
					sizeof(hbuf),NULL,0,NI_NAMEREQD);
			char* haddrp=inet_ntoa(clientaddr.sin_addr);
			printf("server connected to %s (%s)\n",
					hbuf,haddrp);

			doit(connfd);/* Note: parent and child share
					the file descriptor,but address
					space not.*/
			Close(connfd);
		}
	}
	exit(EXIT_SUCCESS);
}
