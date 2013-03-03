#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<math.h>
#define R 10
#define FOOD 1
#define winWidth 500
#define winHeight 200
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
class Point
{
	public:
		int x,y;
		void set(int x,int y)
		{
			Point::x=x;
			Point::y=y;
		}
};
class Snake
{
	public:
		//char *name;
		char name[20];
		int score;
		int cords;  //length of snake
		Point list[20];
		Color color;
		int dir_x,dir_y;		
		void setCoordinates(int ,int);
		void Draw();
		void Update();
		friend ostream& operator<<(ostream &mycout,Snake s);
}snakes[5];
void Snake::setCoordinates(int x,int y)
{
	list[0].set(x,y);
	for(int i=0;i<10;i++)
	{
		x+=R*1.6*dir_x;
		y+=R*1.6*dir_y;
		list[i+1].set(x,y);
	}
	cords=11;
}
ostream &operator<<(ostream &mycout,Snake s)
{
	cout<<endl;
	cout<<"Name\t"<<s.name<<endl;
	cout<<"Score\t"<<s.score<<endl;
	cout<<"Cordinates\t"<<endl;
	for(int i=0;i<s.cords;i++)
		cout<<"("<<s.list[i].x<<","<<s.list[i].y<<"),";
	cout<<"Color\t"<<"("<<s.color.r<<","<<s.color.g<<","<<s.color.b<<")"<<endl;
	cout<<"Direction\t"<<s.dir_x<<","<<s.dir_y<<endl;
	return mycout;
}

void Snake::Update()
{
	for(int i=0;i<cords;i++)
	{
		list[i].x+=dir_x*R*1.6;
		list[i].y+=dir_y*R*1.6;
		if(list[i].x>=winWidth)
			list[i].x-=winWidth;
		else if(list[i].x<=0)
			list[i].x+=winWidth;
		if(list[i].y>=winHeight)
			list[i].y-=winHeight;
		else if(list[i].y<=0)
			list[i].y+=winHeight;
	}
}
int main()
{
	int sockfd;
	int len;
	sockaddr_in address;
	int result;
	int count;
	
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
	read(sockfd,&count,sizeof(int));
	cout<<"Number of clients = "<<count<<endl;
	for(int i=0;i<count;i++)
	{
		cout<<i<<endl;
		read(sockfd,snakes[i].name,sizeof(snakes[i].name));
		read(sockfd,&snakes[i].score,sizeof(snakes[i].score));
		read(sockfd,&snakes[i].color,sizeof(snakes[i].color));
		read(sockfd,&snakes[i].dir_x,sizeof(snakes[i].dir_x));
		read(sockfd,&snakes[i].dir_y,sizeof(snakes[i].dir_y));
		int c[2]; // initial coordinates
		read(sockfd,c,sizeof(c));
		snakes[i].setCoordinates(c[0],c[1]);
		cout<<snakes[i];
	}
	close(sockfd);
	return 0;
}
