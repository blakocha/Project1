#include <iostream>
#include <stdlib.h> //Needed for "exit" function
#include <gl/freeglut.h>
#include <math.h>
//#include <string>
#include <sstream>

using namespace std;

string ftoa(double value) {
	ostringstream o;
	o << value;
	return o.str();
}


int w_width = 480;
int w_height = 480;
int gini_ = 50;
int gini_gran = 200;
int beta_ = 50;
int beta_gran = 200;


void handleKeypress(unsigned char key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	switch (key) {
	case 27: //Escape key
		exit(0); //Exit the program
	case 'k':
		if (gini_ < gini_gran) { gini_ = gini_ + 1; glutPostRedisplay(); } break;
	case 'm':
		if (gini_ > 0) { gini_ = gini_ - 1; glutPostRedisplay(); } break;
	case 'n':
		if (beta_ < beta_gran) { beta_ = beta_ + 1; glutPostRedisplay(); } break;
	case 'j':
		if (beta_ > 0) { beta_ = beta_ - 1; glutPostRedisplay(); } break;
	case 'q':
		exit(0);
	}
}

void triangle_strip()
{
	glBegin(GL_TRIANGLES);//start drawing triangles
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.2f, 0.1f, 0.0f);//triangle one first vertex
	glVertex3f(0.6f, 0.1f, -0.2f);//triangle one second vertex
	glVertex3f(0.4f, 1.0f, 0.0f);//triangle one third vertex
	glEnd();//end drawing of triangles
}

void init()
{
	glClearColor(0, 0, 0, 1.0);
}

void drawGraph(float beta, float gini)
{
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1; t += 0.01) {
		float x = t;
		float y = beta*(pow(x, (1 - gini) / (1 + gini))) + (1 - beta)*(1 - pow((1 - x), (1 + gini) / (1 - gini)));
		glVertex3f(1.6*x - .8, 1.6*y - .8, 0);

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


void display(void)
{

	char* a;
	a = "Use J,N,K,M keys to change shape";

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0); drawGraphW(beta_*1.0 / beta_gran, gini_*1.0 / gini_gran, 0.01);
	//glColor3f(1.0, 0.0, 0.0); drawGraphW(.3, .5, .001);
	//glColor3f(1.0, 1.0, 0.0); drawGraphW(.35, .5, .005);
	//glColor3f(1.0, 0.0, 1.0); drawGraphW(.40, .4, .01);
	//glColor3f(0.0, 1.0, 1.0); drawGraph(beta_*1.0/beta_gran, gini_*1.0/gini_gran);
	//glColor3f(0.0, 1.0, 0.0); drawGraph(.50, .5);
	//glColor3f(0.5, 0.5, 1.0); drawGraph(.6, .5);

	glColor3f(1, 1, 1.0);

	glPushMatrix();
	//glRasterPos3f(0.5, -0.7,-0.5);
	glTranslatef(-.3, -.4, 0);
	glScalef(0.0005, 0.0005, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)a);
	//glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ftoa(gini_*1.0/gini_gran));
	glPopMatrix();

	glPushMatrix();
	//glRasterPos3f(0.5, -0.7,-0.5);
	glTranslatef(-.3, -.6, 0);
	glScalef(0.0005, 0.0005, 1);
	string gini_text2 = ftoa(gini_*1.0 / gini_gran);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)&gini_text2[0u]);
	//glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ftoa(gini_*1.0/gini_gran));
	glPopMatrix();


	glPushMatrix();
	//glRasterPos3f(0.5, -0.7,-0.5);
	glTranslatef(0, -.6, 0);
	glScalef(0.0005, 0.0005, 1);
	string beta_text2 = ftoa(beta_*1.0 / beta_gran);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)&beta_text2[0u]);
	//glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ftoa(gini_*1.0/gini_gran));
	glPopMatrix();


	glutSwapBuffers();
}

void object_3D(void)
{

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(w_width, w_height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Scoringator");
	//glClearColor(0, 0, 0, 1.0);
	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeypress);

	glutMainLoop();
	return 0;
}