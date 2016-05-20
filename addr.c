#include "lib/csapp.h"

int main(int argc,char* argv[]){
	char *buf;
	char *p;
	char arg1[MAXLINE];
	char arg2[MAXLINE];
	char content[MAXLINE];

	int n1=0;
	int n2=0;

	if( (buf=getenv("QUERY_STRING"))!=NULL){
		p=strchr(buf,'&');
		*p='\0';
		strcpy(arg1,buf);
		strcpy(arg2,p+1);
		n1=atoi(arg1);
		n2=atoi(arg2);
	}
	/*Make response body*/
	sprintf(content,"Welcome to add.com: ");
	sprintf(content,"%sThe Internet addition protal.\r\n",content);
	sprintf(content,"%s<p>The answer is : %d + %d = %d</p>\r\n",
			content,n1,n2,n1+n2);
	sprintf(content,"%sThanks for visiting!\r\n",content);

	//printf("Content-Length: %d\r\n",(int)strlen(content));
	//printf("Content-Type: text/html\r\n\r\n");
	printf("Content-Length: %d\r\nContent-Type: text/html\r\n\r\n",
			(int)strlen(content));
	printf("%s",content);
	fflush(stdout);
	exit(EXIT_SUCCESS);
}
