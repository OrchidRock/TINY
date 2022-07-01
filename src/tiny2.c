/*
 *Describe: tiny2.c
 *	This version use the prethreading.
 */
#include "csapp.h"
#include "sbuf.h"
#include "transaction.h"

#define NTHREADS 4
#define SBUFSIZE 16
void* clientThread(void* vargp);
void sigchld_handler(int sig); /* Deal with the SIGCHILD signal */
void sigpipe_handler(int sig){}; /* Deal with the SIGPIPE signal which will
				be send when server write a client that 
				has been closed. */
//static volatile int counter=0;
//static sem_t mutex;
sbuf_t sbuf ;/* */
char root_path[256];    

/*
static void init_client_cnt(){
	counter=0;
	Sem_init(&mutex,0,1);
}
*/


void command(void){
	char buf[MAXLINE];
	if(!Fgets(buf,MAXLINE,stdin)){
		exit(EXIT_SUCCESS);/*EOF*/
	}
	printf("%s",buf);/*Process the input command*/

}
int main(int argc,char* argv[]){

	if(argc<2){
		fprintf(stderr,"Usage: %s [port] [root path]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int listenfd,connfd,clientlen;
	struct sockaddr_in clientaddr;
	pthread_t tid;


	Signal(SIGCHLD, sigchld_handler);
	Signal(SIGPIPE, sigpipe_handler);
	
    sbuf_init(&sbuf,SBUFSIZE);
	//init_client_cnt();

	//struct hostent *hp;
	char hbuf[NI_MAXHOST];

    if(argc >=2 ){
	    listenfd=Open_listenfd(argv[1]);
    }else{
	    listenfd=Open_listenfd("80");
    }

    if(argc >= 3){
        sprintf(root_path,argv[2]);
        //size_t len  = strlen(argv[2]);
        //if(root_path[len-1] != '/' && len < 255){
        //    root_path[len] = '/';
        //    root_path[len+1] = '\0';
        //}
    }else{
        sprintf(root_path,"./");
    }
    //printf("root=%s\n",root_path);
	for(int i=0;i<NTHREADS;i++)  /* Create worker thread*/
		Pthread_create(&tid,NULL,clientThread,NULL);

	while(1){
		clientlen=sizeof(clientaddr);
		hbuf[0]='\0';
		connfd=Accept(listenfd,(SA*)&clientaddr,&clientlen);
		sbuf_insert(&sbuf,connfd); /* maybe sleep here */
		//P(&mutex);
		//counter++;
		//V(&mutex);
		printf("current client count: %d\n",
			 get_sbuf_items_count(&sbuf)); /* maybe sleep*/
		
		Getnameinfo((SA*)&clientaddr,clientlen,hbuf,
				sizeof(hbuf),NULL,0,NI_NAMEREQD);
		char* haddrp=inet_ntoa(clientaddr.sin_addr);/*thread
							     unsafe*/
		printf("server connected to %s (%s)\n",
				hbuf,haddrp);
	}
	sbuf_deinit(&sbuf);
	exit(EXIT_SUCCESS);
}
void* clientThread(void* varfgp){
	Pthread_detach(pthread_self());
	int connfd;
	while(1){
		connfd=sbuf_remove(&sbuf);/* maybe sleep here */
		doit(connfd, root_path);
		Close(connfd);
		//P(&mutex);
		//counter--;
		//V(&mutex);
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
