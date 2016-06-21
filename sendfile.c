#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>


void usage(char* str)
{
	printf("usage:%s,[ip][port][filename]\n",str);
	return;
}

int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		usage(argv[0]);
		exit(0);
	}
	const char* ip=argv[1];
	int port=atoi(argv[2]);
	const char* file_name=argv[3];
	int filefd=open(file_name,O_RDONLY);
	
	assert(filefd>0);
	struct stat stat_buf;
	fstat(filefd,&stat_buf);

	struct sockaddr_in local;
	int sock=socket(AF_INET,SOCK_STREAM,0);
	assert(sock>=0);
	bzero(&local,sizeof(local));
	inet_pton(AF_INET,ip,&local.sin_addr);
	local.sin_port=htons(port);
	
	int ret=bind(sock,(struct sockaddr*)&local,sizeof(local));
	assert(ret!=-1);
	ret=listen(sock,5);
	assert(ret=-1);

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	while(1)
	{
		int connfd=accept(sock,(struct sockaddr*)&client,&len);
		if(connfd<0)
		{
			printf("error\n");
			continue;
		}
		else
		{
			sendfile(connfd,filefd,NULL,stat_buf.st_size);
			close(connfd);
		}
	}
	close(sock);
	return 0;
}
