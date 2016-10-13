/*
 *Describe: tiny.c
 *	A simple ,multi-threads web server that uses 
 *	the GET method to server static and dynamic content.
 */
#include "csapp.h"
#include "transaction.h"
void* clientThread(void* vargp);
void sigchld_handler(int sig); /* Deal with the SIGCHILD signal */
void sigpipe_handler(int sig); /* Deal with the SIGPIPE signal which will
				be send when server write a client that 
				has been closed. */
jmp_buf buf;

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

	/* Install signal handler */
	Signal(SIGCHLD, sigchld_handler);
	Signal(SIGPIPE, sigpipe_handler);
	int listenfd,*connfdp,clientlen;
	struct sockaddr_in clientaddr;
	pthread_t tid;
	//struct hostent *hp;
	//char hbuf[NI_MAXHOST];
	listenfd=Open_listenfd(argv[1]);

	while(1){
		clientlen=sizeof(clientaddr);
	//	hbuf[0]='\0';
		connfdp=Malloc(sizeof(int));
		*connfdp=Accept(listenfd,(SA*)&clientaddr,(socklen_t*)&clientlen);
		/*Create new thread*/
		Pthread_create(&tid,NULL,clientThread,connfdp);

		//Getnameinfo((SA*)&clientaddr,clientlen,hbuf,
				//sizeof(hbuf),NULL,0,NI_NAMEREQD);
		//char* haddrp=inet_ntoa(clientaddr.sin_addr);
		//printf("server connected to %s (%s)\n",
		//	hbuf,haddrp);

	}
	exit(EXIT_SUCCESS);
}
void* clientThread(void* varfgp){
	int connfd=*((int*)varfgp);
	Pthread_detach(pthread_self());
	Free(varfgp);
	int rc  = 0;
	rc = setjmp(buf);
	if(rc == 0){
		doit(connfd);
		Close(connfd);
	}else {
		return NULL;
	}
	return NULL;
}
void sigchld_handler(int sig){
	pid_t pid;
	while((pid = waitpid(-1, NULL, 0)) > 0){}
	if(errno != ECHILD){
		fprintf(stderr, "Server exit unnormally!\n");
		exit(EXIT_FAILURE);
	}
	return;
}
void sigpipe_handler(int sig){
	fprintf(stderr,"Catch the SIGPIPE %s\n", strerror(errno));
	longjmp(buf ,1);
	return;
}
