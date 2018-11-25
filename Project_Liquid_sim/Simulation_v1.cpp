#include <bits/stdc++.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <unistd.h>
//#include "particle.h"

int n_width = 1024;
int n_height = 768;

#define X_MIN_TABLE 212
#define X_MAX_TABLE 812
#define Y_MIN_TABLE 150 

using namespace std;

class particle
{
public:
	float Xcm,Ycm;
	float width;
	float height;
	float Vx,Vy;
	float Ax,Ay;
	//float Fx,Fy;
	float theta;
	float omega,alpha;
	int topLeftx,topLefty,topRightx,topRighty;
	int bottomLeftx,bottomLefty,bottomRightx,bottomRighty;

	void getCorners();

	particle(float X,float Y,float w,float h)
	{
		Xcm = X;
		Ycm = Y;
		width = w;
		height = h;
		Vx = Vy = Ax = Ay = omega = alpha = 0;
		theta = 0;
		getCorners();
	}

	void translateVelocity(float ,float);

	void rotateVelocity(float);
	//void force(float , float );

	void translate();

	void rebound(int ,int ,int ,int,float);

	void rebound(int ,int ,int ,float);

	void rotate();
};

void particle::translateVelocity(float Velx,float Vely)
{
	Vx = Velx;
	Vy = Vely;
}

void particle::rotateVelocity(float Om)
{
	omega = Om;
}

/*void particle::force(float Forcex, float Forcey)
{
	Fx = Forcex;
	Fy = Forcey;
}
*/

void particle::getCorners()
{
	topLeftx = Xcm - width/2;
	topLefty = Ycm + height/2;
	topRightx = Xcm + width/2;
	topRighty = Ycm + height/2;
	bottomLeftx = Xcm - width/2;
	bottomLefty = Ycm - height/2;
	bottomRightx = Xcm + width/2;
	bottomRighty = Ycm - height/2;
}

void particle::translate()
{
	Xcm = Xcm + Vx + 0.5*Ax;
	Ycm = Ycm + Vy + 0.5*Ay;
	Vx = Vx + Ax;
	Vy = Vy + Ay;
}

void particle::rebound(int Xmin,int Ymin,int Xmax,int Ymax,float e)
{
	if(Xcm - Xmin <= width/2 || Xmax - Xcm <= width/2)
	{
		Vx = -e*Vx;
		Vy = (0.5)*(rand()%11 - 5)*Vx + Vy;
	}
	if(Ycm - Ymin <= height/2 || Ymax - Ycm <= height/2)
	{
		Vy = -e*Vy;
		Vx = 0.5*(rand()%11 - 5)*Vy + Vx;
		if(Vy < 0.0001f)
		{
			Ax = Ay = Vx = Vy = 0;
		}
	}	
	if(Xcm >= X_MIN_TABLE && Xcm <= X_MAX_TABLE)
		if(Ycm - Y_MIN_TABLE <= height/2)
		{
			Vy = -e*Vy;
			Vx = 0.5*(rand()%7 - 3)*Vy + Vx;
			if(Vy < 0.01f)
			{
				Ycm = Y_MIN_TABLE + 1;
				Ax = Ay = Vx = Vy = 0;
			}
		}

}

// void particle::rebound(int Xmin,int Xmax,int Ymin,float e)
// {
// 	if(abs(Xcm - Xmin) <= width/2 || abs(Xmax - Xcm) <= width/2)
// 		Vx = -e*Vx;
// 	if(Ycm - Ymin <= height/2)
// 		Vy = -e*Vy;	
// }

void particle::rotate()
{
	glTranslatef(Xcm,Ycm,0.0);
	glRotatef(omega,0,0,1);
	glTranslatef(-Xcm,-Ycm,0.0);
}

vector<particle> p;
//particle p(200.0,200.0,50.0,50.0);

void display()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0.0f,0.0f,0.0f);

	glBegin(GL_QUADS);
		glVertex3i(212,100,0);
		glVertex3i(212,150,0);
		glVertex3i(812,150,0);
		glVertex3i(812,100,0);
		glVertex3i(312,100,0);
		glVertex3i(282,100,0);
		glVertex3i(282,0,0);
		glVertex3i(312,0,0);
		glVertex3i(712,100,0);
		glVertex3i(712,0,0);
		glVertex3i(742,0,0);
		glVertex3i(742,100,0);
		glVertex3i(712,100,0);
	glEnd();


	for(int j = 0;j<80;j++)
		for(int i = 0 ;i<80;i++)
		{
			p[80*j + i].rebound(0,0,n_width,n_height,1.0);

			p[80*j + i].rotate();
			p[80*j + i].translate();
			p[80*j + i].getCorners();
			glBegin(GL_QUADS);
				glColor3f(0.0f,0.0f,1.0f);	
				glVertex3i(p[80*j + i].topLeftx,p[80*j + i].topLefty,0.0f);
				glColor3f(1.0f,0.0f,0.0f);	
				glVertex3i(p[80*j + i].topRightx,p[80*j + i].topRighty,0.0f);
				glColor3f(0.0f,1.0f,0.0f);	
				glVertex3i(p[80*j + i].bottomRightx,p[80*j + i].bottomRighty,0.0f);
				glColor3f(0.5f,0.5f,0.5f);	
				glVertex3i(p[80*j + i].bottomLeftx,p[80*j + i].bottomLefty,0.0f);
	    	glEnd();
		}

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{	
	//Perform Action Based On The Key Pressed
	if(key == 'k')
	{
		for(int i = 0;i<6400;i++)
			p[i].translateVelocity(0.0f,1.0f);
	}
	else if(key == 'g')
	{
		for(int i = 0;i<6400;i++)
			p[i].Ay = -0.025f;
			
	}
/*	else if(key == 'r')
	{
		p.rotateVelocity(0.25f);
	}
	*/
		
}

void reshape(int width, int height) {
    glViewport(0,0,width,height);
    n_width = width;
    n_height = height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GLdouble (width), 0, GLdouble (height) );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
	for(int i = 250;i<410;i += 2)
		for(int j =432;j<592;j +=2)
		{
			particle temp(j,i,2,2);
			p.push_back(temp);
		}
	srand(time(NULL));
	glutInit(&argc, argv);
	
	glutInitWindowSize(n_width,n_height);	//Creates A 900 x 576 Window
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Moving Ball");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  //Allocates Colour Display and Depth Buffers
	glutDisplayFunc(display);		// Register Callback Handler For Display
	glutReshapeFunc(reshape);       // Register Callback Handler For Window Re-Size Event
	glViewport(0, 0, n_width, n_height);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);		// Register Callback Handler For Keyboard Events

	glutMainLoop();
	return 0;
}
