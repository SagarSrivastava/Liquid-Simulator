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
#define Y_TOP_TABLE 150
#define Y_BOTTOM_TABLE 100 
#define X_LEFT_TABLE_LEG 282 
#define X_RIGHT_TABLE_LEG 742
#define X_MIN_BEAKER 437 
#define X_MAX_BEAKER 587
#define Y_MAX_BEAKER 275


bool RED_OPEN = false;
bool GREEN_OPEN = false;
bool BLUE_OPEN = false;

int DIS_POS = 0;

int WATER_LEVEL;
float RED_CONTENT;
float GREEN_CONTENT;
float BLUE_CONTENT;

using namespace std;

void dispOpen(char key);

class particle
{
public:
	float Xcm,Ycm;  // The centre of Mass Co-ordinates of Each particle
	float width;
	float height;   // The height and the width of the particle
	float Vx,Vy;
	float Ax,Ay;	// The acceleration and velocity of the particle
	int topLeftx,topLefty,topRightx,topRighty;
	int bottomLeftx,bottomLefty,bottomRightx,bottomRighty;	//The co-ordinates of the corners
	bool inDispenser; 	//whether the particle is in the dispenser or not
	bool inBeaker; 		//Whether the particle is in the beaker or not
	void getCorners(); // Function to calcuate the corner co-ordinates

	particle(float X,float Y,float w,float h)  //Constructor to initialize the particle
	{
		Xcm = X;
		Ycm = Y;
		width = w;
		height = h;
		Vx = Vy = Ax = Ay = 0;
		inDispenser = true;
		inBeaker = false;
		getCorners();
	}

	//Give a translational velocity to the particle
	void translateVelocity(float ,float);

	//Move the particle
	void translate();

	//Particle's rebound with various objects
	void rebound(int ,int ,int ,int,float);

};

void particle::translateVelocity(float Velx,float Vely)
{
	Vx = Velx;
	Vy = Vely;
}

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

	//Code for Collision with the Window Boundries
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
	if(Xcm >= X_MIN_TABLE && Xcm <= X_MAX_TABLE && Ycm > Y_BOTTOM_TABLE)
		if(Ycm - Y_TOP_TABLE <= height/2)
		{
			float mul;
			int r = rand()%5;
			switch(r)
			{
				case 0: mul = 0.2;
						break;
				case 1: mul = 0.4;
						break;
				case 2: mul = 0.6;
						break;
				case 3: mul = 0.8;
						break;
				case 4: mul = 1.0;
						break;
			}
			Vy = -e*Vy*mul;
			Vx = 0.225*(rand()%11 - 5)*Vy + Vx;
			if(Vy < 0.01f)
			{
				Ycm = Y_TOP_TABLE + 1;
				Ax = Ay = Vx = Vy = 0;
			}
		}
		else if(Xcm <= X_MIN_BEAKER && X_MIN_BEAKER - Xcm <= width/2)
		{
			 Vx = -0.5*(rand()%5+1);
		}
		else if(Xcm >= X_MAX_BEAKER  && Xcm - X_MAX_BEAKER <= width/2)
		{
			Vx = 0.5*(rand()%5+1);
		}
		else if(Xcm >=  X_MIN_BEAKER-1 && Xcm - X_MIN_BEAKER <= width)
		{
			Vx = 0.5*(rand()%5+1);
			inBeaker = true;
		}
		else if(Xcm <= X_MAX_BEAKER+1 && X_MAX_BEAKER - Xcm <= width)
		{
			Vx = -0.5*(rand()%5+1);
			inBeaker = true;
		}	
	if(Ycm >= Y_BOTTOM_TABLE && Ycm <= Y_TOP_TABLE)
		if(Xcm <= X_MIN_TABLE && X_MIN_TABLE - Xcm <= width)
			 Vx = -0.5*(rand()%5+1);
		else if(Xcm >= X_MAX_TABLE && Xcm - X_MAX_TABLE <= width)
			 Vx = 0.5*(rand()%5+1);
	if(Ycm <= Y_BOTTOM_TABLE)
		if(Xcm <= X_LEFT_TABLE_LEG && X_LEFT_TABLE_LEG - Xcm <= width)
			Vx = -0.5*(rand()%5+1);
		else if(Xcm >= X_RIGHT_TABLE_LEG && Xcm - X_RIGHT_TABLE_LEG <= width)
			 Vx = 0.5*(rand()%5+1);

}

vector<particle> red;
vector<particle> green;
vector<particle> blue;

void display()		//Function to display the graphics 
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Table 

	glColor3f(0.36f,0.25f,0.20f);
	
	glBegin(GL_QUADS);
		glVertex3i(X_MIN_TABLE,100,0);
		glVertex3i(X_MIN_TABLE,150,0);
		glVertex3i(X_MAX_TABLE,150,0);
		glVertex3i(X_MAX_TABLE,100,0);
		glVertex3i(312,100,0);
		glVertex3i(X_LEFT_TABLE_LEG,100,0);
		glVertex3i(X_LEFT_TABLE_LEG,0,0);
		glVertex3i(312,0,0);
		glVertex3i(712,100,0);
		glVertex3i(712,0,0);
		glVertex3i(X_RIGHT_TABLE_LEG,0,0);
		glVertex3i(X_RIGHT_TABLE_LEG,100,0);
		glVertex3i(712,100,0);
	glEnd();

	//Dispensers
	glColor3f(0.92549, 0.68235, 0.39608);
	glBegin(GL_QUADS);
		glVertex3i(212+DIS_POS,600,0);
		glVertex3i(212+DIS_POS,650,0);
		glVertex3i(812+DIS_POS,650,0);
		glVertex3i(812+DIS_POS,600,0);
	glEnd();

	glColor3f(0.0f,0.0f,0.0f);

	glLineWidth(3);
	glBegin(GL_LINES);
		glVertex3i(261+DIS_POS,599,0);
		glVertex3i(261+DIS_POS,498,0);
		glVertex3i(261+DIS_POS,498,0);
		glVertex3i(308+DIS_POS,498,0);
		glVertex3i(314+DIS_POS,498,0);
		glVertex3i(361+DIS_POS,498,0);
		glVertex3i(361+DIS_POS,498,0);
		glVertex3i(361+DIS_POS,599,0);
	glEnd();

	glBegin(GL_LINES);
		glVertex3i(461+DIS_POS,599,0);
		glVertex3i(461+DIS_POS,498,0);
		glVertex3i(461+DIS_POS,498,0);
		glVertex3i(508+DIS_POS,498,0);
		glVertex3i(514+DIS_POS,498,0);
		glVertex3i(561+DIS_POS,498,0);
		glVertex3i(561+DIS_POS,498,0);
		glVertex3i(561+DIS_POS,599,0);
	glEnd();

	glBegin(GL_LINES);
		glVertex3i(661+DIS_POS,599,0);
		glVertex3i(661+DIS_POS,498,0);
		glVertex3i(661+DIS_POS,498,0);
		glVertex3i(708+DIS_POS,498,0);
		glVertex3i(714+DIS_POS,498,0);
		glVertex3i(761+DIS_POS,498,0);
		glVertex3i(761+DIS_POS,498,0);
		glVertex3i(761+DIS_POS,599,0);
	glEnd();
	glLineWidth(1);

	//Beakers
	
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
		glVertex3i(X_MIN_BEAKER,275,0);
		glVertex3i(X_MIN_BEAKER,150,0);
		glVertex3i(X_MIN_BEAKER+2,150,0);
		glVertex3i(X_MIN_BEAKER+2,275,0);
		glVertex3i(X_MAX_BEAKER-2,275,0);
		glVertex3i(X_MAX_BEAKER-2,150,0);
		glVertex3i(X_MAX_BEAKER,150,0);
		glVertex3i(X_MAX_BEAKER,275,0);
	glEnd();

	//Particles 

	glColor3f(1.0f,0.0f,0.0f);

	int rcount = 0,gcount = 0,bcount = 0;	

	for(int j = 0;j<50;j++)
	{
		for(int i = 0 ;i<50;i++)
		{
			red[50*j + i].rebound(0,0,n_width,n_height,0.5);

			red[50*j + i].translate();
			red[50*j + i].getCorners();
			if(red[50*j+i].inBeaker)
				rcount++;
			glBegin(GL_QUADS);
				glVertex3i(red[50*j + i].topLeftx + (red[50*j+i].inDispenser?DIS_POS:0),red[50*j + i].topLefty,0.0f);
				glVertex3i(red[50*j + i].topRightx + (red[50*j+i].inDispenser?DIS_POS:0),red[50*j + i].topRighty,0.0f);
				glVertex3i(red[50*j + i].bottomRightx + (red[50*j+i].inDispenser?DIS_POS:0),red[50*j + i].bottomRighty,0.0f);
				glVertex3i(red[50*j + i].bottomLeftx + (red[50*j+i].inDispenser?DIS_POS:0),red[50*j + i].bottomLefty,0.0f);
	    	glEnd();
		}
	}

	glColor3f(0.0f,1.0f,0.0f);	

	for(int j = 0;j<50;j++)
	{
		for(int i = 0 ;i<50;i++)
		{
			green[50*j + i].rebound(0,0,n_width,n_height,0.5);

			green[50*j + i].translate();
			green[50*j + i].getCorners();
			if(green[50*j+i].inBeaker)
				gcount++;
			glBegin(GL_QUADS);
				glVertex3i(green[50*j + i].topLeftx + (green[50*j+i].inDispenser?DIS_POS:0),green[50*j + i].topLefty,0.0f);
				glVertex3i(green[50*j + i].topRightx + (green[50*j+i].inDispenser?DIS_POS:0),green[50*j + i].topRighty,0.0f);
				glVertex3i(green[50*j + i].bottomRightx + (green[50*j+i].inDispenser?DIS_POS:0),green[50*j + i].bottomRighty,0.0f);
				glVertex3i(green[50*j + i].bottomLeftx + (green[50*j+i].inDispenser?DIS_POS:0),green[50*j + i].bottomLefty,0.0f);
	    	glEnd();
		}
	}

	glColor3f(0.0f,0.0f,1.0f);	

	for(int j = 0;j<50;j++)
	{
		for(int i = 0 ;i<50;i++)
		{
			blue[50*j + i].rebound(0,0,n_width,n_height,0.5);

			blue[50*j + i].translate();
			blue[50*j + i].getCorners();
			if(blue[50*j+i].inBeaker)
				bcount++;
			glBegin(GL_QUADS);
				glVertex3i(blue[50*j + i].topLeftx + (blue[50*j+i].inDispenser?DIS_POS:0),blue[50*j + i].topLefty,0.0f);
				glVertex3i(blue[50*j + i].topRightx + (blue[50*j+i].inDispenser?DIS_POS:0),blue[50*j + i].topRighty,0.0f);
				glVertex3i(blue[50*j + i].bottomRightx + (blue[50*j+i].inDispenser?DIS_POS:0),blue[50*j + i].bottomRighty,0.0f);
				glVertex3i(blue[50*j + i].bottomLeftx + (blue[50*j+i].inDispenser?DIS_POS:0),blue[50*j + i].bottomLefty,0.0f);
	    	glEnd();
		}
	}

	int tot_count = rcount + gcount + bcount;

	WATER_LEVEL = Y_TOP_TABLE + (tot_count/100)*3;

	glColor3f((float)rcount/tot_count,(float)gcount/tot_count,(float)bcount/tot_count);

	glBegin(GL_QUADS);
		glVertex3i(X_MIN_BEAKER+2,WATER_LEVEL,0);
		glVertex3i(X_MIN_BEAKER+2,Y_TOP_TABLE,0);
		glVertex3i(X_MAX_BEAKER-2,Y_TOP_TABLE,0);
		glVertex3i(X_MAX_BEAKER-2,WATER_LEVEL,0);
	glEnd();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)	//Function to handle the Keyboard inputs 
{	
	//Perform Action Based On The Key Pressed
	if(key == 'k')
	{
		for(int i = 0;i<2500;i++)
		{
			red[i].translateVelocity(-0.5f,-1.0f);
			green[i].translateVelocity(0.0f,-1.0f);
			blue[i].translateVelocity(0.5f,-1.0f);
		}
	}
	else if(key == 'z')
	{
		for(int i = 0;i<2500;i++)
		{
			red[i].Ay = -0.005f;
			green[i].Ay = -0.005f;
			blue[i].Ay = -0.005f;
		}	
	}
	else if (key == 'a')
	{
		if(DIS_POS == -175);
		else
			DIS_POS--;
	}
	else if( key == 'd')
	{
		if(DIS_POS == 175);
		else
			DIS_POS++;
	}
	else if(key == 'q')
		exit(0);
	else if(key == 'r' || key == 'g' || key == 'b')
	{
		if(key == 'r')
			RED_OPEN = !RED_OPEN;
		else if(key == 'g')
			GREEN_OPEN = !GREEN_OPEN;
		else
			BLUE_OPEN = !BLUE_OPEN;
		dispOpen(key);
	}
		
}

void reshape(int width, int height)   // Function to maintain the aspect ratio when resizing the image.
{
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
	//Initialization of particles of red ,blue and green color respectively.

	for(int i = 500;i<600;i += 2)
		for(int j =262;j<362;j +=2)
		{
			particle temp(j,i,2,2);
			red.push_back(temp);
		}
	for(int i = 500;i<600;i += 2)
		for(int j =462;j<562;j +=2)
		{
			particle temp(j,i,2,2);
			green.push_back(temp);
		}
	for(int i = 500;i<600;i += 2)
		for(int j =662;j<762;j +=2)
		{
			particle temp(j,i,2,2);
			blue.push_back(temp);
		}
	srand(time(NULL));			// To give random velocities to particles 
	glutInit(&argc, argv);
	
	glutInitWindowSize(n_width,n_height);	//Create a window of size n_width x n_height
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Moving Ball");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  //Allocates Colour Display and Double Buffers
	glutDisplayFunc(display);		// Register Callback Handler For Display
	glutReshapeFunc(reshape);       // Register Callback Handler For Window Re-Size Event
	glViewport(0, 0, n_width, n_height);
	glutKeyboardFunc(keyboard);	// Register Callback Handler For Keyboard Events 
	glutIdleFunc(display);		//Callback Handler for Idle state in Loop

	glutMainLoop();				//The main loop call
	return 0;
}


void dispOpen(char key)
{
	if(key == 'r')
	{	
		if(RED_OPEN)
		{
			for(int j = 49;j>=0;j--)
			{
				for(int i = 49;i>=0;i--)
				{
					if(!red[50*j+i].inDispenser)
						continue;
					else
					{
						red[50*j+i].inDispenser = false;
						red[50*j+i].Xcm = 311+DIS_POS;
						red[50*j+i].Ycm = 500;
						red[50*j+i].getCorners();
						red[50*j+i].Ay = -0.005f;
						red[50*j+i].Vx = 0.005*(rand()%3-1);
					}
					display();
				}
			}
		}
	}
	else if(key == 'g')
	{
		if(GREEN_OPEN)
		{
			for(int j = 49;j>=0;j--)
			{
				for(int i = 49;i>=0;i--)
				{
					if(!green[50*j+i].inDispenser)
						continue;
					else
					{
						green[50*j+i].inDispenser = false;
						green[50*j+i].Xcm = 511+DIS_POS;
						green[50*j+i].Ycm = 500;
						green[50*j+i].getCorners();
						green[50*j+i].Ay = -0.005f;
						green[50*j+i].Vx = 0.005*(rand()%3-1);
					}
					display();
				}
			}
		}
	}
	else if(key == 'b')
	{
		if(BLUE_OPEN)
		{
			for(int j = 49;j>=0;j--)
			{
				for(int i = 49;i>=0;i--)
				{
					if(!blue[50*j+i].inDispenser)
						continue;
					else
					{
						blue[50*j+i].inDispenser = false;
						blue[50*j+i].Xcm = 711+DIS_POS;
						blue[50*j+i].Ycm = 500;
						blue[50*j+i].getCorners();
						blue[50*j+i].Ay = -0.005f;
						blue[50*j+i].Vx = 0.005*(rand()%3-1);
					}
					display();
				}
			}
		}
	}
}
