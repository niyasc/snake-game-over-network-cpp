#include<iostream>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h> //fork
#include<netinet/in.h> //accept
#include<pthread.h> //multithreading
using namespace std;
void *serve(void *);
int id=0;
int a=0;
int chance=0;
int count;
int main()
{
	int server_sockfd,client_sockfd;
	socklen_t server_len,client_len;
	sockaddr_in server_address;
	sockaddr_in client_address;
	pthread_t tid;
	cout<<"Enter number of clients "<<endl;
	cin>>count;
	server_sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);
	server_address.sin_port=htons(53000);
	server_len=sizeof(server_address);
	bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
	
	listen(server_sockfd,5);
	
	signal(SIGCHLD,SIG_IGN);
	while(1)
	{
		int d=0;
		//static int count=0;
		std::cout<<"server waiting "<<std::endl;
		
		client_len=sizeof(client_address);
		client_sockfd=accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);
		count--;
		if(pthread_create(&tid, NULL,serve, &client_sockfd)!=0)
		{
			cout<<"Thread error";
			exit(1);
		}
		//count++;
		//cout<<"Waiting for "<<(3-count)<<"more clients "<<endl;
		/*if(clone(()==0)
		{
			while(1)
			{
				sleep(1);
				a++;
				write(client_sockfd,&a,sizeof(a));
			}
			close(client_sockfd);
			exit(0);
		}
		else
		{
			close(client_sockfd);
		}*/
	}
	return 0;
}
void *serve(void *arg)
{
	int id=::id;
	int client_sockfd = *((int*)arg);
	cout<<"my id is "<<id;
	::id++;
	while(count)
	{
		cout<<"\r Waiting for "<<count<<"clients";
	}
	sleep(1);
	while(1)
	{
		sleep(1);
		if(!id)
			a++;
		write(client_sockfd,&a,sizeof(a));
	}
	close(client_sockfd);
}
