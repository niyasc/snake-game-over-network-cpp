#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
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
		Snake();
		~Snake();
		void Draw();
		void Update();
		friend ostream& operator<<(ostream &mycout,Snake s);
}snakes[5];
ostream &operator<<(ostream &mycout,Snake s)
{
	cout<<endl;
	cout<<"Name\t"<<s.name<<endl;
	cout<<"Score\t"<<s.score<<endl;
	cout<<"Cordinates\t"<<endl;
	/*Point *temp=s.list;
	while(temp->next!=NULL)
	{
		cout<<"("<<temp->x<<","<<temp->y<<"),";
		temp=temp->next;
	}*/
	//cout<<"("<<temp->x<<","<<temp->y<<")"<<endl;;
	cout<<"Color\t"<<"("<<s.color.r<<","<<s.color.g<<","<<s.color.b<<")"<<endl;
	cout<<"Direction\t"<<s.dir_x<<","<<s.dir_y<<endl;
	return mycout;
}
	
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
}
Snake::~Snake()
{
	Point *t=list;
	while(t->next!=NULL)
	{
		Point *p=t;
		t=t->next;
		delete p;
	}
	delete t;
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
	int e=read(sockfd,&count,sizeof(int));
	cout<<e<<endl;
	cout<<"abc"<<endl;
	cout<<"Number of clients = "<<count<<endl;
	for(int i=0;i<count;i++)
	{
		read(sockfd,snakes[i].name,sizeof(snakes[i].name));
		read(sockfd,&snakes[i].score,sizeof(snakes[i].score));
		read(sockfd,&snakes[i].color,sizeof(snakes[i].color));
		read(sockfd,&snakes[i].dir_x,sizeof(snakes[i].dir_x));
		read(sockfd,&snakes[i].dir_y,sizeof(snakes[i].dir_y));
		cout<<snakes[i];
		cout<<"out of cout"<<endl;
	}	
	close(sockfd);
	exit(0);
}
