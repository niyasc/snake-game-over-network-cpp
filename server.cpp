#include<iostream>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h> //fork
#include<netinet/in.h> //accept
#include<pthread.h> //multithreading
#include<math.h>
#define winWidth 500
#define winHeight 200
#define CLIENTS 2
#define R 10
#define FOOD 1
using namespace std;
class Color
{
	public :
		float r,g,b;
};
class Food
{
	public:
		Color color;
		int x,y;
		void Generate()
		{
			color.r=(float)((random()%4)/4.0);
			color.g=(float)((random()%4)/4.0);
			color.b=(float)((random()%4)/4.0);
			x=random()%winWidth;
			y=random()%winHeight;
		}
		Food()
		{
			color.r=(float)((random()%4)/4.0);
			color.g=(float)((random()%4)/4.0);
			color.b=(float)((random()%4)/4.0);
			x=random()%winWidth;
			y=random()%winHeight;
		}
}f[FOOD];
class Snake
{
	public:
		//char *name;
		char name[20];
		int score;
		int c[2];
		Color color;
		int dir_x,dir_y;		
		Snake();
		void Draw();
		void Update();
}snakes[5];
Snake::Snake()
{
	int x,y;
	score=0;
	dir_x=0;
	dir_y=0;
	srandom(random()%500);
	do
	{
		x=random()%500;
		y=random()%500;
	}while(x<=100||y<=100);
	c[0]=x;
	c[1]=y;
	switch(random()%4)
	{
		case 0:dir_x=+1;
			//color.r==color.g=
			break;
		case 1:dir_x=-1;
			break;
		case 2:dir_y=+1;
			break;
		case 3:dir_y=-1;
			break;
	}
	color.r=(float)((random()%4)/4.0);
	color.g=(float)((random()%4)/4.0);
	color.b=(float)((random()%4)/4.0);
}		
void *serve(void *);  //client serving function prototype
int id=0;//client id
int count=0;//Number of clients connected
int main()
{
	int server_sockfd,client_sockfd;
	socklen_t server_len,client_len;
	sockaddr_in server_address;
	sockaddr_in client_address;
	pthread_t tid;
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
		count++;
		if(pthread_create(&tid, NULL,serve, &client_sockfd)!=0)
		{
			cout<<"Thread error";
			exit(1);
		}
	}
	return 0;
}
void *serve(void *arg)
{
	static int id=::id;
	int client_sockfd = *((int*)arg);
	::id++;
	while(count<CLIENTS)
	{
		cout<<"\r Waiting for "<<(3-count)<<"clients";
	}
	sleep(1);
	cout<<"Number of clients= "<<count<<endl;
	cout<<"my sockfd"<<client_sockfd<<endl;
	write(client_sockfd,&count,sizeof(count));
	for(int i=0;i<CLIENTS;i++)
	{
		//write(client_sockfd,&id,sizeof(d));
		//write(client_sockfd,&snakes[i],sizeof(snakes[id]));
		write(client_sockfd,snakes[i].name,sizeof(snakes[i].name));
		write(client_sockfd,&snakes[i].score,sizeof(snakes[i].score));
		write(client_sockfd,&snakes[i].color,sizeof(snakes[i].color));
		write(client_sockfd,&snakes[i].dir_x,sizeof(snakes[i].dir_x));
		write(client_sockfd,&snakes[i].dir_y,sizeof(snakes[i].dir_y));
		write(client_sockfd,snakes[i].c,sizeof(snakes[i].c));
		sleep(1);
	}
	close(client_sockfd);
	pthread_exit(arg);
}
