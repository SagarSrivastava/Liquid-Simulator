#include <bits/stdc++.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <unistd.h>
//#include "particle.h"

int n_width = 640;
int n_height = 480;

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
	}	
	if(Ycm - Ymin <= height/2 || Ymax - Ycm <= height/2)
	{
		Vy = -e*Vy;
	}	
}

void particle::rotate()
{
	glTranslatef(Xcm,Ycm,0.0);
	glRotatef(omega,0,0,1);
	glTranslatef(-Xcm,-Ycm,0.0);
}

particle p(200.0,200.0,50.0,50.0);

void display()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//glColor3f(0.0f,0.0f,0.0f);


/*		glVertex3i(100,0,0);
		glVertex3i(100,150,0);
		glVertex3i(100,150,0);
		glVertex3i(85,150,0);
		glVertex3i(200,0,0);
		glVertex3i(200,150,0);
		glVertex3i(200,150,0);
		glVertex3i(215,150,0);
	glEnd();
*/
	glColor3f(1.0f,0.0f,0.0f);	

	p.rebound(0,0,n_width,n_height,1.0);

	p.translate();
	p.rotate();
	p.getCorners();
	glBegin(GL_QUADS);
		glVertex3i(p.topLeftx,p.topLefty,0.0f);
		glVertex3i(p.topRightx,p.topRighty,0.0f);
		glVertex3i(p.bottomRightx,p.bottomRighty,0.0f);
		glVertex3i(p.bottomLeftx,p.bottomLefty,0.0f);
    glEnd();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{	
	//Perform Action Based On The Key Pressed
	if(key == 'k')
	{
		p.translateVelocity(0.7f,0.8f);
	}
	else if(key == 'r')
	{
		p.rotateVelocity(0.25f);
	}
		
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

void init()
{	
	// glEnable(GL_DEPTH_TEST);									//Enables Depth Testing
	 glDepthMask(GL_TRUE);
	 glDepthFunc(GL_LEQUAL);
	 glDepthRange(0.0f, 1.0f);

	glShadeModel(GL_SMOOTH);				// Enables Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Sets Default Clear Colour
	glClearDepth(1.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	glutInitWindowSize(n_width,n_height);	//Creates A 900 x 576 Window
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Moving Ball");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DOUBLE | GLUT_RGBA);  //Allocates Colour Display and Depth Buffers
	glutDisplayFunc(display);		// Register Callback Handler For Display
	glutReshapeFunc(reshape);       // Register Callback Handler For Window Re-Size Event
	glViewport(0, 0, n_width, n_height);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);		// Register Callback Handler For Keyboard Events

	init();

	glutMainLoop();
	return 0;
}
