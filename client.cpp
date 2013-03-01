#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
using namespace std;
int main()
{
	int sockfd;
	int len;
	sockaddr_in address;
	int result;
	int d;
	
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("127.0.0.1");
	address.sin_port=htons(53000);
	len=sizeof(address);

	result=connect(sockfd,(struct sockaddr *)&address,len);
	if(result==-1)
	{
		cout<<"Failed to connect"<<endl;
		exit(1);
	}
	//write(sockfd,&ch,1);
	while(1)
	{
		read(sockfd,&d,sizeof(int));
		cout<<"Server says "<<d<<endl;
	}
	close(sockfd);
	exit(0);
}
