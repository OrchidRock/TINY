/*
 *Describe: tiny2.c
 *	This version use the prethreading.
 */
#include "lib/csapp.h"
#include "lib/sbuf.h"
#include "transaction.h"

#define NTHREADS 4
#define SBUFSIZE 16
void* clientThread(void* vargp);
//static volatile int counter=0;
//static sem_t mutex;
sbuf_t sbuf ;/* */

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

	if(argc!=2){
		fprintf(stderr,"Usage: %s <port>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int listenfd,connfd,clientlen;
	struct sockaddr_in clientaddr;
	pthread_t tid;
	
	sbuf_init(&sbuf,SBUFSIZE);
	//init_client_cnt();

	//struct hostent *hp;
	char hbuf[NI_MAXHOST];
	listenfd=Open_listenfd(argv[1]);

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
		doit(connfd);
		Close(connfd);
		//P(&mutex);
		//counter--;
		//V(&mutex);
	}
	return NULL;
}
