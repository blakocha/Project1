#include <iostream>
#include <stdlib.h> //Needed for "exit" function
#include <gl/freeglut.h>
#include <math.h>
//#include <string>
#include <sstream>

using namespace std;

string ftoa(double value, string unit="") {
	ostringstream o;
	o << value <<unit;
	return o.str();
}

int w_width = 640; int w_height = 480;
int inp_flag=0; int inp_size=6;

int gini_ = 35124; int gini_gran = 100000; int gini_step=500;
int gini2_ = 55451;
int badrate_ = 30001; int badrate_gran = 100000; int badrate_step=500;
int apprate_ = 70001; int apprate_gran = 100000; int apprate_step=500;
int beta_ = 251; int beta_gran = 1000; int beta_step=5;
int steps_ = 0; int steps_gran = 9; int steps_step=1;

string inp_string [6] = {"GINI1:", "GINI2:", "Beta:", "Total bad rate:", "Approval rate:", "Step size (pp):"};
int inp_value [6] = {35124, 55451, 251, 30001, 70001, 0};
int inp_gran [6] = {100000, 100000, 1000, 100000, 100000, 9};
int inp_step [6] = {500, 500, 5, 500, 500, 1};
float inp_xs [6] = {.02, .02, .02, .6, .6, .6};
float inp_x [6] = {.2, .2, .2, 1.1, 1.1, 1.1};
float inp_y [6] = {-.1, -.2, -.3, -.1, -.2, -.3};
bool inp_numstep[6] = {false,false,false,false,false,true};
float numsteps[10] = {1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01,0.005, 0.002, 0.001};

string numstepsize(int value, string unit="") {
	ostringstream o;
	o << numsteps[value] <<unit;
	return o.str();
}

void handleKeypress(unsigned char key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	switch (key) {
	case 27: //Escape key
		exit(0); //Exit the program
	case 9:
		inp_flag = (inp_flag + 1)%inp_size ; glutPostRedisplay(); break;
	case 'q':
		exit(0);
	}
}


void handleSpecialpress(int key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	switch (key) {
	case GLUT_KEY_UP:
		if (inp_value[inp_flag] < inp_gran[inp_flag]) { inp_value[inp_flag] = min(inp_value[inp_flag] + inp_step[inp_flag],inp_gran[inp_flag]); glutPostRedisplay(); } break;
	case GLUT_KEY_DOWN:
		if (inp_value[inp_flag] > 0) { inp_value[inp_flag]= max(0,inp_value[inp_flag]- inp_step[inp_flag]); glutPostRedisplay(); } break;
	case GLUT_KEY_RIGHT:
		if (inp_value[inp_flag] < inp_gran[inp_flag]) { inp_value[inp_flag] = min(inp_value[inp_flag] + 1,inp_gran[inp_flag]); glutPostRedisplay(); } break;
	case GLUT_KEY_LEFT:
		if (inp_value[inp_flag] > 0) {inp_value[inp_flag]= max(0,inp_value[inp_flag]- 1); glutPostRedisplay(); } break;
	}
}



void init()
{
	glClearColor(0, 0, 0, 1.0);
}

void drawGraphArea()
{
	glColor3f(.8, .8, .8);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 0, 0);
	glEnd();

glPushAttrib(GL_ENABLE_BIT);
glLineStipple(3, 0xAAAA);
glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES); glVertex3f(0, .2, 0); glVertex3f(1, .2, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(0, .4, 0); glVertex3f(1, .4, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(0, .6, 0); glVertex3f(1, .6, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(0, .8, 0); glVertex3f(1, .8, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(.2, 0, 0); glVertex3f(.2, 1, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(.4, 0, 0); glVertex3f(.4, 1, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(.6, 0, 0); glVertex3f(.6, 1, 0); glEnd();
	glBegin(GL_LINES); glVertex3f(.8, 0, 0); glVertex3f(.8, 1, 0); glEnd();

glPopAttrib();

}

void drawtext(float x_pos, float y_pos, float scale, string text)
{
	glPushMatrix();
	glTranslatef(x_pos, y_pos, 0);
	glScalef(scale*.8, scale, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)&text[0u]);
	glPopMatrix();
}

void drawInputSelection()
{
    float w1=0.02;
    glPushMatrix();
    glTranslatef(-.9,.8,0);
	glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]-w1, 0);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]+.1-w1, 0);
        glVertex3f(inp_x[inp_flag]+.4-w1, inp_y[inp_flag]+.1-w1, 0);
        glVertex3f(inp_x[inp_flag]+.4-w1, inp_y[inp_flag]-w1, 0);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]-w1, 0);
    glEnd();
    glPopMatrix();

}


void drawInputArea()
{
    glPushMatrix();
    glTranslatef(-.9,.8,0);

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -0.35, 0);
        glVertex3f(1.8, -0.35, 0);
        glVertex3f(1.8, 0, 0);
	glEnd();

    drawtext(0.02,.05,0.0005, "Inputs:");

    int i=0;
    for (i=0;i<inp_size;i++) {
    drawtext(inp_xs[i],inp_y[i],0.0005, inp_string[i]);
    if (inp_numstep[i])
        {drawtext(inp_x[i],inp_y[i],0.0005, numstepsize(inp_value[i]," pp"));}
    else
        {drawtext(inp_x[i],inp_y[i],0.0005, ftoa(inp_value[i]*100.0 / inp_gran[i], "%"));}
    }

    glPopMatrix();
}


void drawOutputArea()
{
    glPushMatrix();
    glTranslatef(-.1,.3,0);

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -1.1, 0);
        glVertex3f(1.05, -1.1, 0);
        glVertex3f(1.05, 0, 0);
	glEnd();

    drawtext(0.02,.05,0.0005, "Outputs:");
    drawtext(0.02,-.1,0.0005, "Bad rate reduction:");
    drawtext(0.65,-.1,0.0005, "-99.999%");
    drawtext(0.02,-.2,0.0005, "Approval increase:");
    drawtext(0.65,-.2,0.0005, "+99.999%");

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -.3, 0);
        glVertex3f(1.05, -.3, 0);
        glVertex3f(1.05, 0, 0);
	glEnd();

    int outputstep=90;
    drawtext(0.02,-.4,0.00050, "Initial bad rate in approved:");
    drawtext(0.8,-.4,0.00050, "99.999%");
    drawtext(0.02,-.4-outputstep*1/1000.0,0.00045, "Reduced bad rate in approved:");
    drawtext(0.8,-.4-outputstep*1/1000.0,0.00050, "99.999%");
    drawtext(0.02,-.4-outputstep*2/1000.0,0.00050, "Increased approval rate:");
    drawtext(0.8,-.4-outputstep*2/1000.0,0.00050, "99.999%");
    drawtext(0.02,-.4-outputstep*3/1000.0,0.00050, "Initial GINI on approved:");
    drawtext(0.8,-.4-outputstep*3/1000.0,0.00050, "99.999%");
    drawtext(0.02,-.4-outputstep*4/1000.0,0.00050, "New GINI on approved");
    drawtext(0.02,-.4-outputstep*5/1000.0,0.00050, "    (bad rate reduction):");
    drawtext(0.8,-.4-outputstep*5/1000.0,0.00050, "99.999%");
    drawtext(0.02,-.4-outputstep*6/1000.0,0.00050, "New GINI on approved:");
    drawtext(0.02,-.4-outputstep*7/1000.0,0.00050, "    (approval reduction):");
    drawtext(0.8,-.4-outputstep*7/1000.0,0.00050, "99.999%");

	glPopMatrix();
}

void drawGraph(float beta, float gini, float width)
{
    glLineWidth(width);
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1; t += 0.01) {
		float x = t;
		float y = beta*(pow(x, (1 - gini) / (1 + gini))) + (1 - beta)*(1 - pow((1 - x), (1 + gini) / (1 - gini)));
		glVertex3f(x, y, 0);
	}
	glEnd();
}

void drawGraphW(float beta, float gini, float width)
{
	float gran = 0.001;
	float t, x, y, x_pop = 0, y_pop = -gran, mn;

	glBegin(GL_TRIANGLE_STRIP);
	for (t = 0; t <= 1; t += gran) {
		x = t;
		y = beta*(pow(x, (1 - gini) / (1 + gini))) + (1 - beta)*(1 - pow((1 - x), (1 + gini) / (1 - gini)));
		mn = width / sqrt(pow(x - x_pop, 2) + pow(y - y_pop, 2));
		glVertex3f(1.6*x - .8 + mn*(y - y_pop), 1.6*y - .8 - mn*(x - x_pop), 0);
		glVertex3f(1.6*x - .8 - mn*(y - y_pop), 1.6*y - .8 + mn*(x - x_pop), 0);
		x_pop = x;
		y_pop = y;
	}
	glEnd();
}


void drawGraphWB(float beta, float gini, float width)
{

	float gran = 0.001;
	float t, x, y, x_pop = 0, y_pop = -gran, mn;

	glBegin(GL_TRIANGLE_STRIP);
	for (t = 0; t <= 1; t += gran) {
		x = t;
		y = beta*(pow(x, (1 - gini) / (1 + gini))) + (1 - beta)*(1 - pow((1 - x), (1 + gini) / (1 - gini)));
		mn = width / sqrt(pow(x - x_pop, 2) + pow(y - y_pop, 2));
		glVertex3f(x + mn*(y - y_pop), y - mn*(x - x_pop), 0);
		glVertex3f(x -  - mn*(y - y_pop), y + mn*(x - x_pop), 0);
		x_pop = x;
		y_pop = y;
	}
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
        glTranslatef(-.95, -.8, 1);
        glScalef(.8, .8*w_width*1.0/w_height, 1);
        drawGraphArea();
        glColor3f(0.0, 0.0, 1.0); drawGraph(beta_*1.0 / beta_gran, gini_*1.0 / gini_gran, 4);
        glColor3f(1.0, 0.0, 0.0); drawGraph(beta_*1.0 / beta_gran, gini2_*1.0 / gini_gran, 4);
	glPopMatrix();

    glLineWidth(1);
	glColor3f(.2, .2, .2);
    drawInputSelection();
	glColor3f(.8, .8, 0);
    drawInputArea();
    drawOutputArea();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(w_width, w_height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Scoringator");
	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialpress);

	glutMainLoop();
	return 0;
}
