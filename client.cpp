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
int a=0;
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
		Point *next;
		Point(int x,int y)
		{
			Point::x=x;
			Point::y=y;
			next=NULL;
		}
};
class Snake
{
	public:
		//char *name;
		char name[20];
		int score;
		Point *list;
		Color color;
		int dir_x,dir_y;		
		//Snake();
		~Snake();
		void setCoordinates(int ,int);
		void Draw();
		void Update();
		friend ostream& operator<<(ostream &mycout,Snake s);
};
void Snake::setCoordinates(int x,int y)
{
	Point *t=new Point(x,y);
	list=t;
	for(int i=0;i<10;i++)
	{
		x+=R*1.6*dir_x;
		y+=R*1.6*dir_y;
		Point *p=new Point(x,y);
		t->next=p;
		t=p;
	}
	t->next=NULL;
}
ostream &operator<<(ostream &mycout,Snake s)
{
	cout<<endl;
	cout<<"Name\t"<<s.name<<endl;
	cout<<"Score\t"<<s.score<<endl;
	cout<<"Cordinates\t"<<endl;
	Point *temp=s.list;
	while(temp->next!=NULL)
	{
		cout<<"("<<temp->x<<","<<temp->y<<"),";
		temp=temp->next;
	}
	cout<<"("<<temp->x<<","<<temp->y<<")"<<endl;;
	cout<<"Color\t"<<"("<<s.color.r<<","<<s.color.g<<","<<s.color.b<<")"<<endl;
	cout<<"Direction\t"<<s.dir_x<<","<<s.dir_y<<endl;
	return mycout;
}
Snake::~Snake()
{
	Point *t=list;
	cout<<"a="<<a<<endl;
	int i=0;
	while(t->next!=NULL)
	{
		Point *p=t;
		t=t->next;
		cout<<"("<<p->x<<","<<p->y<<")"<<endl;
		delete p;
		cout<<i++<<endl;
	}
	cout<<"out of loop";
	cout<<"("<<t->x<<","<<t->y<<")"<<endl;
	delete t;
	cout<<"deleted t"<<endl;
}
		
void Snake::Update()
{
	Point *t;
	t=list;
	list=list->next;
	free(t);
	t=list;
	while(t->next!=NULL)
		t=t->next;
	Point *p=new Point(t->x+dir_x*R*1.6,t->y+dir_y*R*1.6);
	t->next=p;
	t=t->next;
	if(t->x>=winWidth)
		t->x-=winWidth;
	if(t->y>=winHeight)
		t->y-=winHeight;
	if(t->x<=0)
		t->x+=winWidth;
	if(t->y<=0)
		t->y+=winHeight;
	//do touch food?
	for(int i=0;i<FOOD;i++)
	{
		if(sqrt((f[i].x-t->x)*(f[i].x-t->x)+(f[i].y-t->y)*(f[i].y-t->y))<2*R)
		{
			f[i].Generate();
			score+=10;
			if(score%30==0)
			{
				t=list;
				while(t->next!=NULL)
					t=t->next;
				Point *p=new Point(t->x+dir_x*R*1.6,t->y+dir_y*R*1.6);
				t->next=p;
				t=t->next;
				if(t->x>=winWidth)
					t->x-=winWidth;
				if(t->y>=winHeight)
					t->y-=winHeight;
				if(t->x<=0)
					t->x+=winWidth;
				if(t->y<=0)
					t->y+=winHeight;
			}	
		}
	}
}
int main()
{
	int sockfd;
	int len;
	sockaddr_in address;
	int result;
	int count;
	Snake snakes[5];
	
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
	cout<<"Going to read socket"<<endl;
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
		//cout<<snakes[i];
		cout<<"out of cout"<<endl;
	}
	a++;
	cout<<"out of loop"<<endl;
	cout<<sockfd<<endl;
	close(sockfd);
	return 0;
}
