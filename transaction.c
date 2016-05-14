#include "lib/csapp.h"
#include "transaction.h"

static void
clienterror(int fd,char* cause,char* errnum,
		char* shortmessage,char* longmessage) {
	char buf[MAXLINE],body[MAXBUF];

	/*Build the http response body*/
	sprintf(body,"<html><title>Tiny Error</title>\r\n");
	sprintf(body,"%s<body bgcolor=""ffffff"">\r\n",body);
	sprintf(body,"%s%s: %s\r\n",body,errnum,shortmessage);
	sprintf(body,"%s<p>%s: %s</p>\r\n",body,longmessage,cause);
	sprintf(body,"%s<hr><em>The Tiny Web Server</em></hr>\r\n",body);
	sprintf(body,"%s</body></html>",body);

	/*Print the HTTP response*/
	sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmessage);
	Rio_writen(fd,buf,strlen(buf));
	sprintf(buf,"Content-Type: text/html\r\n");
	Rio_writen(fd,buf,strlen(buf));
	sprintf(buf,"Content-Length: %d\r\n\r\n",(int)strlen(body));
	Rio_writen(fd,buf,strlen(buf));
	Rio_writen(fd,body,strlen(body));
}
/* read and ignore request message header*/
static void 
read_requesthdrs(rio_t* rp){
	char buf[MAXLINE];
	Rio_readlineb(rp,buf,MAXLINE);
	while(strcmp(buf,"\r\n")){
		Rio_readlineb(rp,buf,MAXLINE);
		printf("%s",buf);
	}

}

static int
parse_uri(char* uri,char* filename,char* cgiargs){
	char *ptr;
	if(!strstr(uri,"cgi-bin")){ /*Static content*/
		strcpy(cgiargs,"");
		strcpy(filename,".");
		strcat(filename,uri);
		if(uri[strlen(uri)-1]=='/'){
			strcat(filename,"home.html");
		}
		return 1; /* static */
	}else{ /* Dynamic content*/
		/*get args*/
		ptr=index(uri,'?');
		if(ptr){
			strcpy(cgiargs,ptr+1);
			*ptr='\0';
		}else
			strcpy(cgiargs,"");
		strcpy(filename,".");
		strcat(filename,uri);
		return 0;
	}
	

}
/* drive file type from file name*/
static void 
getfile_type(char* filename,char* filetype){
	if(strstr(filename,".html"))
		strcpy(filetype,"text/html");
	else if(strstr(filename,".gif"))
		strcpy(filetype,"text/gif");
	else if(strstr(filename,".jpg"))
		strcpy(filetype,"text/jpeg");
	else
		strcpy(filetype,"text/plain");
}
static void
server_static(int fd,char* filename,int filesize){
	int srcfd;
	char* srcp;
	char filetype[MAXLINE],buf[MAXBUF];
#ifdef DEBUG
	printf("----debug : server_static----");
	printf("%s filetype: %s size: %d\n",filename,filetype,filesize);
#endif
	/*Send response header to client*/	
	getfile_type(filename,filetype);
	sprintf(buf,"HTTP/1.0 200 OK\r\n");
	sprintf(buf,"%sServer: Tiny Web Server\r\n",buf);
	sprintf(buf,"%sContent-Length: %d\r\n",buf,filesize);
	sprintf(buf,"%sContent-Type: %s\r\n\r\n",buf,filetype);

	Rio_writen(fd,buf,strlen(buf));

	/* Send response body to client*/
	srcfd=Open(filename,O_RDONLY,0);
	srcp=Mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
	Close(srcfd);
#ifdef DEBUG
	printf("----debug : server_static----");
	printf("%s content:\n",filename);
	printf("%s\n",srcp);
#endif
	Rio_writen(fd,srcp,filesize);
	Munmap(srcp,filesize);
}

static void
server_dynamic(int fd,char* filename,char* cgiargs){
	char buf[MAXLINE];
	char* emptylist[]={NULL};
	/*Return response*/
	sprintf(buf,"HTTP/1.0 200 OK\r\n");
	Rio_writen(fd,buf,strlen(buf));
	sprintf(buf,"Server: Tiny Web Server\r\n");
	Rio_writen(fd,buf,strlen(buf));
	if(Fork()==0){ /* Child process*/
		/*Set all CGI vars*/
		setenv("QUERY_STRING",cgiargs,1);
		Dup2(fd,STDOUT_FILENO);/* Redirect stdout to client*/
		Execve(filename,emptylist,environ); /*Run CGI program*/
	}
	Wait(NULL);
}

void 
doit(int fd){
	int is_static;
	struct stat sbuf;
	char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
	char filename[MAXLINE],cgiargs[MAXLINE];
	rio_t rio;

	/*Read request line and headerse*/
	Rio_readinitb(&rio,fd);
	Rio_readlineb(&rio,buf,MAXLINE);
	sscanf(buf,"%s %s %s",method,uri,version);
	if (strcmp(method,"GET")){
		clienterror(fd,method,"501","Not Implemented",
				"Tiny does not implement this method");
		return;
	}
	read_requesthdrs(&rio);
#ifdef DEBUG
	printf("----debug : doit----\n");
	printf("method= %s,uri= %s,version= %s\n",method,uri,version);
#endif
	/*prase uri*/

	is_static=parse_uri(uri,filename,cgiargs);
#ifdef DEBUG
	printf("----debug : doit----\n");
	printf("is_static= %s,uri= %s,filename= %s, cgiargs= %s\n",
		is_static?"static":"dynamic",uri,filename,cgiargs);
#endif

	if(stat(filename,&sbuf)<0){
		clienterror(fd,filename,"404","Not Found",
				"Tiny could't find this file");
		return;
	}
	if(is_static){
		if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)){
			clienterror(fd,filename,"403","Forbidden",
					"Tiny couldn't read the file");
			return;
		}
		server_static(fd,filename,sbuf.st_size);
	}
	else{ /* Server dynamic content*/
		if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)){
			clienterror(fd,filename,"403","Forbidden",
					"Tiny couldn't run the CGI program");
			return;
		}
		server_dynamic(fd,filename,cgiargs);
	}
}
