#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>
#include<assert.h>
#include<pthread.h>
#include <mysql/mysql.h> 
#include"persist.h"
#include"my_recv.h"

#define PORT        4507
#define MAXLINE     1024
#define LISTENQ     5
#define SIZE        10


typedef struct server_context_st
{
	int cli_cnt;        /*客户端个数*/
	int clifds[SIZE];   /*客户端的个数*/
	fd_set allfds;      /*句柄集合*/
	int maxfd;          /*句柄最大值*/
} server_context_st;
typedef struct pthread_count
{
	pthread_t thid[SIZE];
	int 	  pthread_cnt;	
}pthread_count;

server_context_st *s_srv_ctx = NULL;
pthread_count *pcount=NULL; 
MYSQL *mysql;

int server_init();
int create_server_proc(int port);
void handle_client_proc(int srvfd);
void recv_client_msg(fd_set *readfds);
int handle_client_msg(int fd, char *buf);
int accept_client_proc(int srvfd);
void server_uninit();
void* transit(int clifd);
void mysql_connect(MYSQL *mysql);

int main(int argc,char *argv[])
{
	int  srvfd;
	mysql = mysql_init(NULL);
	if (!mysql) 
	{
        my_err("mysql_init", __LINE__);
    }
    mysql_connect(mysql);
	/*初始化服务端context*/
	if (server_init() < 0) 
	{
		return -1;
	}
	/*创建服务,开始监听客户端请求*/
	srvfd = create_server_proc(PORT);
	if (srvfd < 0) 
	{
		if (s_srv_ctx) 
		{
			free(s_srv_ctx);
			s_srv_ctx = NULL;
		}
		my_err("create socket",__LINE__);
	}
	/*开始接收并处理客户端请求*/
	handle_client_proc(srvfd);
	
	close_connection(mysql);
    return 0;  
}

int server_init()
{
	s_srv_ctx = (server_context_st *)malloc(sizeof(server_context_st));
	if (s_srv_ctx == NULL) 
	{
		return -1;
	}
	pcount=(pthread_count*)malloc(sizeof(pthread_count));
	if(pcount==NULL)
	{
		return -1;
	}
	memset(pcount,0,sizeof(pthread_count));
	memset(s_srv_ctx, 0, sizeof(server_context_st));

	int i = 0;
	for (;i < SIZE; i++) 
	{
		s_srv_ctx->clifds[i] = -1;
	}

	return 0;
}

int create_server_proc(int port)
{
	int  fd;
	struct sockaddr_in servaddr;
	fd = socket(AF_INET, SOCK_STREAM,0);
	if (fd == -1) 
	{
		fprintf(stderr, "create socket fail,erron:%d,reason:%s\n",errno, strerror(errno));
		return -1;
	}

	/*一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。*/
	int reuse = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) 
	{
		return -1;
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if (bind(fd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1) 
	{
		perror("bind error: ");
		return -1;
	}

	listen(fd,LISTENQ);

	return fd;
}
void handle_client_proc(int srvfd)
{
	int  clifd = -1;
	int  retval = 0;
	fd_set *readfds = &s_srv_ctx->allfds;
	struct timeval tv;
	int i = 0;

	while (1) 
	{
		/*每次调用select前都要重新设置文件描述符和时间，因为事件发生后，文件描述符和时间都被内核修改啦*/
		FD_ZERO(readfds);
		/*添加监听套接字*/
		FD_SET(srvfd, readfds);
		s_srv_ctx->maxfd = srvfd;

		tv.tv_sec = 30;
		tv.tv_usec = 0;

		/*开始轮询接收处理服务端和客户端套接字*/
		retval = select(s_srv_ctx->maxfd + 1, readfds, NULL, NULL, &tv);
		if (retval == -1) 
		{
			fprintf(stderr, "select error:%s.\n", strerror(errno));
			return;
		}
		if (retval == 0) {
			fprintf(stdout, "select is timeout.\n");
			continue;
		}
		if (FD_ISSET(srvfd, readfds)) /*测试srvfd是否可读 */
		{
			/*监听客户端请求*/
			accept_client_proc(srvfd);
		}
	}
}

int accept_client_proc(int srvfd)
{
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	cliaddrlen = sizeof(cliaddr);
	int clifd = -1;

	printf("accpet clint proc is called.\n");

ACCEPT:
	clifd = accept(srvfd,(struct sockaddr*)&cliaddr,&cliaddrlen);

	if (clifd == -1) 
	{
		if (errno == EINTR) 
		{
			goto ACCEPT;
		} 
		else 
		{
			fprintf(stderr, "accept fail,error:%s\n", strerror(errno));
			return -1;
		}
	}

	fprintf(stdout, "accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);

	//将新的连接描述符添加到数组中

	s_srv_ctx->clifds[s_srv_ctx->cli_cnt++] = clifd;

	if (s_srv_ctx->cli_cnt== SIZE) 
	{
		fprintf(stderr,"too many clients.\n");
		return -1;
	}
	/*创建线程处理接受*/
	if(pthread_create(&(pcount->thid[(pcount->pthread_cnt)++]),NULL,(void*)transit,(void*)clifd)!=0)
	{
		my_err("create pthread",__LINE__);
	}
	
}

void recv_client_msg(fd_set *readfds)
{
	int i = 0, n = 0;
	int clifd;
	char buf[MAXLINE] = {0};
	for (i = 0;i <= s_srv_ctx->cli_cnt;i++) 
	{
		clifd = s_srv_ctx->clifds[i];
		if (clifd < 0) 
		{
			continue;
		}
		/*判断客户端套接字是否有数据*/
		if (FD_ISSET(clifd, readfds)) 
		{
			//接收客户端发送的信息
			n = recv(clifd,buf,MAXLINE,0);
			if (n <= 0) 
			{
				/*n==0表示读取完成，客户都关闭套接字*/
				FD_CLR(clifd, &s_srv_ctx->allfds);
				close(clifd);
				s_srv_ctx->clifds[i] = -1;
				continue;
			}
			handle_client_msg(clifd, buf);
		}
	}
}

int handle_client_msg(int fd, char *buf) 
{
	assert(buf!=NULL);
	printf("recv buf is :%s\n", buf);
	//write(fd, buf, strlen(buf) +1);
	return 0;
}
void* transit(int clifd)
{
	char recvbuf[32];
	if(recv(clifd,recvbuf,32,0)<0)
	{
		my_err("recv",__LINE__);
	}
	if(strcmp("1",recvbuf)==0)
		reg_account_per(clifd,mysql);
	else if(strcmp("2",recvbuf)==0)
		/**/;
	else
	{
		printf("%d client closed\n",clifd);
		close(clifd);
		s_srv_ctx->cli_cnt--;
	}
}
void server_uninit()
{
	if (s_srv_ctx) 
	{
		free(s_srv_ctx);
		s_srv_ctx = NULL;
	}
}
