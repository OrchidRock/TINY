#include "csapp.h"

int main(int argc,char* argv[]){
	char *buf;
	char *p;
	char arg1[MAXLINE];
	char arg2[MAXLINE];
	char content[MAXLINE];

	int X = 0;
	int Y = 0;

	if( (buf=getenv("QUERY_STRING"))!=NULL){
		p=strchr(buf,'&');
		*p='\0';
		strcpy(arg1,buf);
		strcpy(arg2,p+1);
		X=atoi(strchr(arg1, '=')+1);
		Y=atoi(strchr(arg2, '=')+1);
	}
	/*Make response body*/
	sprintf(content,"Welcome to add.com: ");
	sprintf(content,"%sThe Internet addition protal.\r\n",content);
	sprintf(content,"%s<p>The answer is : %d + %d = %d</p>\r\n",
			content, X, Y, X+Y);
	sprintf(content,"%sThanks for visiting!\r\n",content);

	//printf("Content-Length: %d\r\n",(int)strlen(content));
	//printf("Content-Type: text/html\r\n\r\n");
	printf("Content-Length: %d\r\nContent-Type: text/html\r\n\r\n",
			(int)strlen(content));
	printf("%s",content);
	fflush(stdout);
	fclose(stdout);
	exit(EXIT_SUCCESS);
}
