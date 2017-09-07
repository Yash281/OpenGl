#include <GL/glut.h>
#include<vector>
#include"Point.h"
#include <iostream>

#define LINE 1
#define RECTANGLE 2
#define POLYGON 3
#define CIRCLE 4
#define CLEAR 5
#define EDIT 6
#define STOP_EDITING 7

GLfloat pointsize = 5;
GLfloat start_MOUSEx, start_MOUSEy;
GLfloat cx, cy;
GLfloat end_MOUSEx, end_MOUSEy;
GLfloat RED[3] = { 1,0,0 };
GLfloat sizes[2];     
GLfloat step;         
GLfloat SIDE = 50;

int draw = 0;
static int track = 0;
bool edit = false;
bool point = false;


std::vector<Point> linePoints;
std::vector<Point> rectPoints;
std::vector<Point> circlePoints;
std::vector<Point> polygonPoints;
Point*currentpoint;

unsigned int currentDrawMode = LINE;


int inwindow(int x, int y)
{
	return x,y;
}

void drawLines()
{
	glColor3fv(RED);

	glPointSize(pointsize);
	glBegin(GL_POINTS);
	
	for (int i = 0; i < linePoints.size(); i += 2)
	{
		glVertex2f(linePoints[i].getx(), linePoints[i].gety());
		glVertex2f(linePoints[i + 1].getx(), linePoints[i + 1].gety());
	}
	glEnd();
	
	glBegin(GL_LINES);

	glColor3fv(RED);

	for (int i = 0; i < linePoints.size(); i+=2)
	{
		glVertex2f(linePoints[i].getx(), linePoints[i].gety());
		glVertex2f(linePoints[i + 1].getx(), linePoints[i + 1].gety());
	}
	glEnd();

}

void Rekt()
{
	glColor3fv(RED);
	glPointSize(pointsize);
	glColor3fv(RED);
	glBegin(GL_POINTS);

	
	for (int i = 0; i < rectPoints.size(); i += 2)
	{
		glVertex2f(rectPoints[i].getx(), rectPoints[i].gety());
		glVertex2f(rectPoints[i+1].getx(), rectPoints[i + 1].gety());
	}
	glEnd();

	glColor3fv(RED);

	
	glBegin(GL_LINES);

	glColor3fv(RED);

	for (int i = 0; i < rectPoints.size(); i += 2)
	{
		glVertex2f(rectPoints[i].getx(), rectPoints[i].gety());
		glVertex2f(rectPoints[i].getx(), rectPoints[i + 1].gety());

		glVertex2f(rectPoints[i + 1].getx(), rectPoints[i + 1].gety());
		glVertex2f(rectPoints[i + 1].getx(), rectPoints[i].gety());

		glVertex2f(rectPoints[i].getx(), rectPoints[i + 1].gety());
		glVertex2f(rectPoints[i + 1].getx(), rectPoints[i + 1].gety());

		glVertex2f(rectPoints[i].getx(), rectPoints[i].gety());
		glVertex2f(rectPoints[i + 1].getx(), rectPoints[i].gety());
	}
	glEnd();

}

void Circle()
{
	int i;
	int triangleAmount = 20;

	for (i = 0; i<circlePoints.size(); i += 2)
	{
		float a = circlePoints[i+1].getx();
		float b = circlePoints[i+1].gety();
		float c = circlePoints[i].getx();
		float d = circlePoints[i].gety();
		
		float radius = sqrtf((a - c)*(a - c) + (b - d)*(b - d));
		
		GLfloat twicePi = 2.0f * 3.14f;
		
		glColor3fv(RED);
		glBegin(GL_POINTS);
		glPointSize(5);
		
		glVertex2f(circlePoints[i].getx(),circlePoints[i].gety());
		glVertex2f(circlePoints[i+1].getx(), circlePoints[i+1].gety());
		
		glEnd();
		glBegin(GL_LINE_LOOP);
		
		for (int i = 0; i < triangleAmount; i++) {
			glVertex2f(
				c + (radius * cosf(i *  twicePi / triangleAmount)),
				d + (radius * sinf(i * twicePi / triangleAmount))
			);
			
		}
	}
	glEnd();


}

void Polygon()
{
	glColor3fv(RED);
	glPointSize(pointsize);
	glColor3fv(RED);
	glBegin(GL_POINTS);


	for (int i = 0; i < polygonPoints.size(); i += 2)
	{
		glVertex2f(polygonPoints[i].getx(), polygonPoints[i].gety());
	}
	glEnd();

	glColor3fv(RED);

	glPointSize(pointsize);
	glBegin(GL_LINES);

	for (int i = 1; i <polygonPoints.size(); i++)
	{
		glVertex2f(polygonPoints[i].getx(), polygonPoints[i].gety());
		glVertex2f(polygonPoints[i-1].getx(), polygonPoints[i-1].gety());
	}
	
	glEnd();

}


void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	drawLines();
	Rekt();
	Circle();
	Polygon();

	glEnd();
	glFlush();
}

void MenuEvents(int option)
{
	switch (option)
	{
	case LINE:
		currentDrawMode = LINE;
		break;
	case RECTANGLE:
		currentDrawMode = RECTANGLE;
		break;
	case CIRCLE:
		currentDrawMode = CIRCLE;
		break;
	case POLYGON:
		currentDrawMode = POLYGON;
		break;
	case CLEAR:
		linePoints.clear();
		rectPoints.clear();
		circlePoints.clear();
		polygonPoints.clear();
		break;
	case EDIT:
		edit = true;
		break;
	case STOP_EDITING:
		edit = !edit;
		break;
	default:
		break;
	}
}

int CreateGLUTMenus()
{
	int menu;

	menu = glutCreateMenu(MenuEvents);

	glutAddMenuEntry("Lines", LINE);
	glutAddMenuEntry("Rectangle", RECTANGLE);
	glutAddMenuEntry("Polygon", POLYGON);
	glutAddMenuEntry("Circle", CIRCLE);
	glutAddMenuEntry("Clear", CLEAR);
	glutAddMenuEntry("Edit", EDIT);
	glutAddMenuEntry("Stop Editing", STOP_EDITING);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return menu;
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void mouse(int btn, int state, int x, int y)
{
	if (!edit) {
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (currentDrawMode == LINE)
			{
				linePoints.push_back(Point(x, y));

			}
			if (currentDrawMode == RECTANGLE)
			{
				rectPoints.push_back(Point(x, y));
			}
			if (currentDrawMode == POLYGON)
			{
				polygonPoints.push_back(Point(x, y));
			}
			if (currentDrawMode == CIRCLE)
			{
				circlePoints.push_back(Point(x, y));
			}


		}
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			if (currentDrawMode == LINE)
			{
				linePoints.push_back(Point(x, y));
			}
			if (currentDrawMode == RECTANGLE)
			{
				rectPoints.push_back(Point(x, y));
			}
			if (currentDrawMode == POLYGON)
			{
				polygonPoints.push_back(Point(x, y));
			}
			if (currentDrawMode == CIRCLE)
			{
				circlePoints.push_back(Point(x, y));
			}
		}
	}
		else
		{
			if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				for (int i = 0; i < linePoints.size(); i++)
				{
					if (linePoints[i].getx() > x - 20 && linePoints[i].gety() < x + 20 && linePoints[i].gety() > y - 20 && linePoints[i].gety() < y + 20)
					{
						std::cout << "point";
						currentpoint = &linePoints[i];

						break;
					}
				}
				if (!point)
				{
					for (int i = 0; i < rectPoints.size(); i++)
					{
						if (rectPoints[i].getx() > x - 20 && rectPoints[i].gety() < x + 20 && rectPoints[i].gety() > y - 20 && rectPoints[i].gety() < y + 20)
						{
							std::cout << "point";
							currentpoint = &rectPoints[i];

							break;
						}
					}
				}
				if (!point)
				{
					for (int i = 0; i < circlePoints.size(); i++)
					{
						if (circlePoints[i].getx() > x - 20 && circlePoints[i].gety() < x + 20 && circlePoints[i].gety() > y - 20 && circlePoints[i].gety() < y + 20)
						{
							std::cout << "point";
							currentpoint = &circlePoints[i];

							break;
						}
					}
				}
				if (!point)
				{
					for (int i = 0; i < polygonPoints.size(); i++)
					{
						if (polygonPoints[i].getx() > x - 20 && polygonPoints[i].gety() < x + 20 && polygonPoints[i].gety() > y - 20 && polygonPoints[i].gety() < y + 20)
						{
							std::cout << "point";
							currentpoint = &polygonPoints[i];

							break;
						}
					}
				}
			}
			if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
			{


				if (currentpoint != nullptr)
				{
					std::cout << "position at " << x << " " << y;
					currentpoint->setx(x);
					currentpoint->sety(y);
				}

			}
		}
		
	}

	
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(1, 1);
	glutCreateWindow("Moving squares");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	CreateGLUTMenus();
	//glutIdleFunc(spindisplay);
	glutMainLoop();
}