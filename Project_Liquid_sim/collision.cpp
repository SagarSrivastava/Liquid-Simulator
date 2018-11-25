#include <bits/stdc++.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
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
		Vx = -e*Vx;
	if(Ycm - Ymin <= height/2 || Ymax - Ycm <= height/2)
		Vy = -e*Vy;
}

void particle::rebound(int Xmin,int Xmax,int Ymin,float e)
{
	if(abs(Xcm - Xmin) <= width/2 || abs(Xmax - Xcm) <= width/2)
		Vx = -e*Vx;
	if(Ycm - Ymin <= height/2)
		Vy = -e*Vy;	
}
void particle::rotate()
{
	glTranslatef(Xcm,Ycm,0.0);
	glRotatef(omega,0,0,1);
	glTranslatef(-Xcm,-Ycm,0.0);
}

particle p1(200.0,200.0,50.0,50.0);

particle p2(400.0,300.0,50.0,50.0);


// void collision()
// {
	
// }

void display()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0.0f,0.0f,0.0f);


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

	p1.rebound(0,0,n_width,n_height,1.0);
	//p.rebound(100.0,200.0,0.0,1.0);

	p1.translate();
	p1.rotate();
	p1.getCorners();
	glBegin(GL_QUADS);
		glVertex3i(p1.topLeftx,p1.topLefty,0.0f);
		glVertex3i(p1.topRightx,p1.topRighty,0.0f);
		glVertex3i(p1.bottomRightx,p1.bottomRighty,0.0f);
		glVertex3i(p1.bottomLeftx,p1.bottomLefty,0.0f);
    glEnd();

    glColor3f(0.0f,0.0f,1.0f);

    p2.rebound(0,0,n_width,n_height,1.0);
	//p.rebound(100.0,200.0,0.0,1.0);

	p2.translate();
	p2.rotate();
	p2.getCorners();
	glBegin(GL_QUADS);
		glVertex3i(p2.topLeftx,p2.topLefty,0.0f);
		glVertex3i(p2.topRightx,p2.topRighty,0.0f);
		glVertex3i(p2.bottomRightx,p2.bottomRighty,0.0f);
		glVertex3i(p2.bottomLeftx,p2.bottomLefty,0.0f);
    glEnd();


    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{	
	//Perform Action Based On The Key Pressed
	if(key == 'k')
	{
		p1.translateVelocity(0.5f,0.5f);
	}
	else if(key == 'j')
	{
		p2.translateVelocity(0.7f,0.3f);
	}
	else if(key == 'e')
	{
		p2.rotateVelocity(0.045f);
	}
	else if(key == 'r')
	{
		p1.rotateVelocity(0.025f);
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

int main(int argc, char **argv)
{
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
