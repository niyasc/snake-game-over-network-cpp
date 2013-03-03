#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#define R 10
#define PI 3.14
#define FOOD 4
int fcount;
GLint winWidth=800;
GLint winHeight=600;
int id;
void init()
{
	glClearColor(1.0,1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,winWidth,0,winHeight);

}
void circle(float x, float y, float r) 
{ 
	float delta_theta = 0.01;
	glBegin(GL_POINTS);
		glVertex2i(24,33);
	glEnd();
	glBegin( GL_POLYGON ); // OR GL_LINE_LOOP

		for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f(x+ r*cos(angle), y+r*sin(angle), 0 );

	glEnd();
}
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
}snake[5];
void Snake::setCoordinates(int x,int y)
{
	list[0].set(x,y);
	for(int i=0;i<10;i++)
	{
		x+=R*1.6*dir_x;
		y+=R*1.6*dir_y;
		list[i+1].set(x,y);
	}
	//dir_x=0;
	//dir_y=1;
	cords=11;
}
void Snake::Draw()
{
	for(int i=0;i<cords;i++)
		circle(list[i].x,list[i].y,R);
				
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

int sockfd;
int len;
sockaddr_in address;
int result;
int count;

void getInitial()
{
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
		read(sockfd,snake[i].name,sizeof(snake[i].name));
		read(sockfd,&snake[i].score,sizeof(snake[i].score));
		read(sockfd,&snake[i].color,sizeof(snake[i].color));
		cout<<snake[i];
	}
	//close(sockfd);
}
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i=0;i<count;i++)
	{
		glColor3f(snake[i].color.r,snake[i].color.g,snake[i].color.b);
		snake[i].Draw();
	}
	for(int i=0;i<fcount;i++)
	{
		glColor3f(f[i].color.r,f[i].color.g,f[i].color.b);
		circle(f[i].x,f[i].y,R);
	}
	glutSwapBuffers();
	glFlush();
}
void p1(GLint key,GLint x,GLint y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:	if(snake[id].dir_x!=1)
						snake[id].dir_x=-1;
					snake[id].dir_y=0;
					break;
		case GLUT_KEY_RIGHT:	if(snake[id].dir_x!=-1)
						snake[id].dir_x=1;
					snake[id].dir_y=0;
					break;
		case GLUT_KEY_UP:	if(snake[id].dir_y!=-1)
						snake[id].dir_y=1;
					snake[id].dir_x=0;
					break;
		case GLUT_KEY_DOWN:	if(snake[id].dir_y!=1)	
						snake[id].dir_y=-1;
					snake[id].dir_x=0;
	}
	int dir[2];
	dir[0]=snake[id].dir_x;
	dir[1]=snake[id].dir_y;
	cout<<dir[0]<<dir[1];
	write(sockfd,dir,sizeof(dir));
}
struct Cordxy
{
	int x,y;
};
void repeat()
{
	Cordxy cordxy[20];
	for(int i=0;i<count;i++)
	{
		read(sockfd,&snake[i].score,sizeof(snake[i].score));
		read(sockfd,&snake[i].cords,sizeof(snake[i].cords));
		cout<<snake[i].cords<<endl;
		read(sockfd,cordxy,sizeof(cordxy));
		for(int j=0;j<snake[i].cords;j++)
		{
			snake[i].list[j].set(cordxy[j].x,cordxy[j].y);
			cout<<cordxy[j].x<<","<<cordxy[j].y<<endl;
		}
					
	}
	read(sockfd,&fcount,sizeof(fcount));
	for(int i=0;i<fcount;i++)
	{
		read(sockfd,&f[i].color,sizeof(f[i].color));
		Cordxy pos;
		read(sockfd,&pos,sizeof(pos));
		f[i].x=pos.x;
		f[i].y=pos.y;
	}
	glColor3f(1,1,1);
	//display();
	//output(100,100,text);
	usleep(1000);
	draw();
}
/*void winReshapeFcn(GLint newWidth,GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(0,winWidth,0,winHeight);
}*/
void reshape (int w, int h)
{
	// Window size has changed stuff goes here.
	gluOrtho2D (0, w,0,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
}
int main(int argc,char *argv[])
{
	glutInit(&argc,argv);	
	init();
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(winWidth,winHeight);

	
	//glutFullScreen();
	getInitial();//get snake details from server
	glutCreateWindow("Snake Game");
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutSpecialFunc(p1);
	glutIdleFunc(repeat);	
	glutMainLoop();
}
