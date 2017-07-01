#include <iostream>
#include <stdlib.h> //Needed for "exit" function
#include <gl/freeglut.h>
#include <math.h>
//#include <string>
#include <sstream> //to make sure ftoa function is working correctly amoung others
#include <algorithm> //to make sure min and max are working correctly
#include <vector> // vector is used to change precision in numeric calculations
#include <iomanip> // to make setprecision work in ftoa function...

using namespace std;

typedef vector<double> vec;

//two technical true/false flags for debug (printing debug to the console) and float precision problems workaround
bool printdebug=false;
bool workaround=true;

//precision of output and input (number of decimal points
int prec_input=3;
int prec_output=4;
int prec_outputmain=6;

int temp_x=0;
int temp_y=0;

//local version of float to text with specified unit and precision
string ftoa(double value, string unit="", int prec=1) {
	ostringstream o;
	if (value>=100) {prec=3;} // workaround to not exceed the frames
	o << fixed << setprecision(prec) <<value <<unit;
	return o.str();
}

//char to txt for digit printing in an input box
string ctoa(unsigned char digit, string unit="") {
	ostringstream o;
	o << digit;
	return o.str();
}

//window size
int w_width = 640; int w_height = 480;
int act_width = 640; int act_height = 480;

int inp_flag=0; //input flag - current input to be highlighted
int inp_size=6; //number of inputs
bool inp_click=false; //if true, mouseclick should activate inputbox

//input parameters
string inp_string [6] = {"GINI1:", "GINI2:", "Beta:", "Total bad rate:", "Approval rate:", "Step size (pp):"};
int inp_value [6] = {55000, 60000, 500, 25000, 55000, 4};
int inp_gran [6] = {100000, 100000, 1000, 100000, 100000, 9};
int inp_min [6] = {0, 0, 0, 1, 1, 0};
int inp_max [6] = {100000, 100000, 1000, 99999, 99999, 9};
int inp_step [6] = {500, 500, 5, 500, 500, 1};
float inp_xs [6] = {.02, .02, .02, .7, .7, .7};
float inp_x [6] = {.25, .25, .25, 1.15, 1.15, 1.15};
float inp_y [6] = {-.1, -.2, -.3, -.1, -.2, -.3};
float inp_format1 [6] = {100, 100, 1, 100, 100, 1};
string inp_format2 [6] = {"%", "%", "", "%", "%", " pp"};
bool inp_numstep[6] = {false,false,false,false,false,true};
float numsteps[10] = {1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01,0.005, 0.002, 0.001};
bool inputbox_flag=false; //a flag if input box is active
string inputbox_string=""; //string being input
int inputbox_string_len=0; //length of the string being input
bool inputbox_comma=false; //if a coma has been input
int inp_flag_prev=0;
int inp_value_prev=inp_value[inp_flag_prev];
bool inp_undo_flag=false;

//output parameters
double outp_badratereduction=-.99999999;
double outp_approvalincrease=.99999;
double outp_badrate1=.99999;
double outp_badrate2=.99999;
double outp_apprate3=.99999;
double outp_gini1=.99999;
double outp_gini2=.99999;
double outp_gini3=.99999;

//colors
void colorChartOutline() {glColor3f(.8, .8, .8);}
void colorChartLine1() {glColor3f(0.0, 0.0, 1.0);}
void colorChartLine2() {glColor3f(1.0, 0.0, 0.0);}
void colorInputSelection() {glColor3f(.4, .4, .4);}
void colorInputArea() {glColor3f(.8, .8, .4);}
void colorHelpArea() {glColor3f(.8, .8, .8);}
void colorInputText() {glColor3f(0.0,0.0,0.0);}

//a function zeroing the current inputbox
void inputNullify() {
		inputbox_string=""; inputbox_string_len=0; inputbox_flag=false;
		inputbox_comma=false;
}


string numstepsize(int value, string unit="") {
	ostringstream o;
	o << numsteps[value] <<unit;
	return o.str();
}

void inputConfirm() {
	    if (inputbox_string_len>0) {
        inp_flag_prev=inp_flag;
        inp_value_prev=inp_value[inp_flag_prev];
	    inp_value[inp_flag]=static_cast<int>(roundf(atof(inputbox_string.c_str())*inp_gran[inp_flag]/inp_format1[inp_flag]));
        inp_value[inp_flag]=min(max(inp_value[inp_flag], inp_min[inp_flag]),inp_max[inp_flag]);}
        inputNullify();
}

void undo() {
    int temp_inp_value;
    if (inputbox_flag==false) {
    inp_flag=inp_flag_prev;
    temp_inp_value=inp_value[inp_flag_prev];
    inp_value[inp_flag_prev]=inp_value_prev;
    inp_value_prev=temp_inp_value;
    glutPostRedisplay();
    }
}

void handleKeypress(unsigned char key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
    int mod;

    mod=glutGetModifiers(); //to check if Shift is pressed

	switch (key) {

	case 27: //Escape key
		inputNullify(); glutPostRedisplay(); break;

	case 9: //tab
        if (mod==GLUT_ACTIVE_SHIFT) //if Shift is pressed -> go backwards
        {if (inp_flag==0)
            {inp_flag=inp_size-1;}
            else {inp_flag=inp_flag-1;}
        }
        else {inp_flag = (inp_flag + 1)%inp_size ; }; //if shift is not pressed -> proceed +1
		inputNullify(); //input is cancelled - without enter!
		glutPostRedisplay(); break;

    case 'z': //Ctrl+Z to undo
                undo();
        break;

	case 8: //backspace pressed - cancel input
		if (inputbox_string_len>0)
            {inputbox_string.pop_back();
            inputbox_string_len--;
            glutPostRedisplay(); }
        break;

	case 13: //enter pressed
        inputConfirm();
        glutPostRedisplay(); break;
	case 'q':
		exit(0);
	}

	if (key>='0' && key<='9') {
            inputbox_flag=true;
            if (inp_numstep[inp_flag]==false) {
            inputbox_string=inputbox_string+ctoa(key);
            inputbox_string_len++; }
            glutPostRedisplay();};

	if (key=='.' || key==',') {
            if (inp_numstep[inp_flag]==false) {
            if (inputbox_comma==false)
			{
			inputbox_flag = true;
			inputbox_comma=true;
            if (inputbox_string=="") {
				inputbox_string = inputbox_string + '0' + '.';
				inputbox_string_len=inputbox_string_len+2;
			}
			else {
				inputbox_string = inputbox_string + '.';
				inputbox_string_len++;
			}
			}
			glutPostRedisplay();};
	}
}

void handleSpecialpress(int key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates

	if (inputbox_flag==false)
	{

	switch (key) {
	case GLUT_KEY_UP:
        inp_flag_prev=inp_flag;
        inp_value_prev=inp_value[inp_flag_prev];
		inp_value[inp_flag] = min(inp_value[inp_flag] + inp_step[inp_flag],inp_max[inp_flag]);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
        inp_flag_prev=inp_flag;
        inp_value_prev=inp_value[inp_flag_prev];
		inp_value[inp_flag]= max(inp_min[inp_flag],inp_value[inp_flag]- inp_step[inp_flag]); glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT:
        inp_flag_prev=inp_flag;
        inp_value_prev=inp_value[inp_flag_prev];
		inp_value[inp_flag] = min(inp_value[inp_flag] + 1,inp_max[inp_flag]); glutPostRedisplay(); break;
	case GLUT_KEY_LEFT:
        inp_flag_prev=inp_flag;
        inp_value_prev=inp_value[inp_flag_prev];
		inp_value[inp_flag]= max(inp_min[inp_flag],inp_value[inp_flag]- 1); glutPostRedisplay(); break;
	}
	}
}



void drawGraphArea()
{
	colorChartOutline();
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

void drawtext2(float x_pos, float y_pos, float scale, string text)
{
	glPushMatrix();
	glTranslatef(x_pos, y_pos, 0);
	glScalef(scale*.8, scale, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)&text[0u]);
	glPopMatrix();
}

void drawtext(float x_pos, float y_pos, float scale, string text)
{
	glPushMatrix();
	glRasterPos2f(x_pos, y_pos);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)&text[0u]);
	glPopMatrix();
}


void drawtexth(float x_pos, float y_pos, float scale, string text)
{
	glPushMatrix();
	glRasterPos2f(x_pos, y_pos);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)&text[0u]);
	glPopMatrix();
}


void drawInputBox()
{
    float w1=.02;
    glPushMatrix();
    glTranslatef(-.9,.7,0);
	glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]-w1, 0);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]+.1-w1, 0);
        glVertex3f(inp_x[inp_flag]+.4-w1, inp_y[inp_flag]+.1-w1, 0);
        glVertex3f(inp_x[inp_flag]+.4-w1, inp_y[inp_flag]-w1, 0);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]-w1, 0);
    glEnd();

    colorInputText();
    glLineWidth(2);
    drawtext(inp_x[inp_flag],inp_y[inp_flag],0.0005, inputbox_string);
    glLineWidth(1);
    glPopMatrix();

}



void drawInputSelection()
{
    float w1=0.02;
    glPushMatrix();
    glTranslatef(-.9,.7,0);
	glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]-w1, 0);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]+.1-w1, 0);
        glVertex3f(inp_x[inp_flag]+.3-w1, inp_y[inp_flag]+.1-w1, 0);
        glVertex3f(inp_x[inp_flag]+.3-w1, inp_y[inp_flag]-w1, 0);
        glVertex3f(inp_x[inp_flag]-w1, inp_y[inp_flag]-w1, 0);
    glEnd();
    glPopMatrix();

}


void drawInputArea()
{
    glPushMatrix();
    glTranslatef(-.9,.7,0);

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -0.35, 0);
        glVertex3f(1.6, -0.35, 0);
        glVertex3f(1.6, 0, 0);
	glEnd();

    drawtext(0.02,.05,0.0005, "Inputs:");

    int i=0;
    for (i=0;i<inp_size;i++) {
    drawtext(inp_xs[i],inp_y[i],0.0005, inp_string[i]);
    if (inp_numstep[i])
        {drawtext(inp_x[i],inp_y[i],0.0005, numstepsize(inp_value[i],inp_format2[i]));}
    else
        {drawtext(inp_x[i],inp_y[i],0.0005, ftoa(inp_value[i]*inp_format1[i] / inp_gran[i], inp_format2[i],prec_input));}
    }

    glPopMatrix();
}


void drawHelpArea()
{
    glPushMatrix();
    glTranslatef(-.6,1.05,0);

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -.3, 0);
        glVertex3f(1.55, -.3, 0);
        glVertex3f(1.55, 0, 0);
	glEnd();

	if (inputbox_flag==true)
        if (inp_numstep[inp_flag]){
            drawtext(0.05,-.13,2,"Use ARROWS to change the step in numeric calculations.");
            drawtext(0.05,-.23,2,"Press ENTER to close this message.");}
        else
            {drawtext(0.05,-.13,2,"Use digits and a dot. Press ENTER to confirm. ");}
    else {
	drawtexth(0.05,-.06,2,"Use TAB / SHIFT+TAB to select the input variable to be changed. To change ");
	drawtexth(0.05,-.13,2,"the value of the input variable: (1) use arrows RIGHT/LEFT for small changes, ");
	drawtexth(0.05,-.20,2,"UP/DOWN for bigger ones or (2) use numeric keys, dot as decimal point + ENTER. ");
	drawtexth(0.05,-.27,2,"Option (2) not available for step size. Press 'Z' to undo, 'Q' to quit. ");
	}

	glPopMatrix();
}

void drawOutputArea()
{
    glPushMatrix();
    glTranslatef(-.1,.2,0);

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -1.05, 0);
        glVertex3f(1.075, -1.05, 0);
        glVertex3f(1.075, 0, 0);
	glEnd();

    drawtext(0.02,.05,0.0005, "Outputs:");
    drawtext(0.02,-.1,0.0005, "Bad rate reduction:");
    drawtext(0.65,-.1,0.0005, ftoa(outp_badratereduction*100, "%", prec_outputmain));
    drawtext(0.02,-.2,0.0005, "Approval increase:");
    drawtext(0.65,-.2,0.0005, ftoa(outp_approvalincrease*100, "%", prec_outputmain));

	glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -.25, 0);
        glVertex3f(1.075, -.25, 0);
        glVertex3f(1.075, 0, 0);
	glEnd();

    int outputstep=90;
    drawtext(0.01,-.35,0.00050, "Initial bad rate in approved:");
    drawtext(0.81,-.35,0.00050, ftoa(outp_badrate1*100, "%", prec_output));
    drawtext(0.01,-.35-outputstep*1/1000.0,0.00045, "Reduced bad rate in approved:");
    drawtext(0.81,-.35-outputstep*1/1000.0,0.00050, ftoa(outp_badrate2*100, "%", prec_output));
    drawtext(0.01,-.35-outputstep*2/1000.0,0.00050, "Increased approval rate:");
    drawtext(0.81,-.35-outputstep*2/1000.0,0.00050, ftoa(outp_apprate3*100, "%", prec_output));
    drawtext(0.01,-.35-outputstep*3/1000.0,0.00050, "Initial GINI on approved:");
    drawtext(0.81,-.35-outputstep*3/1000.0,0.00050, ftoa(outp_gini1*100, "%", prec_output));
    drawtext(0.01,-.35-outputstep*4/1000.0,0.00050, "New GINI on approved");
    drawtext(0.01,-.35-outputstep*5/1000.0,0.00050, "    (bad rate reduction):");
    drawtext(0.81,-.35-outputstep*5/1000.0,0.00050, ftoa(outp_gini2*100, "%", prec_output));
    drawtext(0.01,-.35-outputstep*6/1000.0,0.00050, "New GINI on approved:");
    drawtext(0.01,-.35-outputstep*7/1000.0,0.00050, "    (approval increase):");
    drawtext(0.81,-.35-outputstep*7/1000.0,0.00050, ftoa(outp_gini3*100, "%", prec_output));

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

void handleMouseclick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    if (inputbox_flag==false && inp_click) {inputbox_flag=true;}
    else inputConfirm();
  }
}


void handleMouseover(int x, int y)
{
    temp_x=x;
    temp_y=y;

    int i=0;
    inp_click=false;
    for (i=0;i<inp_size;i++)
    {
        if (inp_x[i]*320+30<=temp_x*w_width/act_width && temp_x*w_width/act_width<=inp_x[i]*320+30+94 && inp_y[i]*(-240)+78<=temp_y*w_height/act_height && temp_y*w_height/act_height<=inp_y[i]*(-240)+78+21)
            {if (inp_flag!=i) {inputNullify(); inp_flag=i;} inp_click=true; }
    }

    glutPostRedisplay();
}

void handleReshape(int newWidth, int newHeight )
{
    act_width=newWidth; act_height=newHeight;
    glViewport(0,0,newWidth,newHeight);
    glutPostRedisplay();
}


void recalculate(float gini1, float gini2, float beta, float badrate, float apprate, float stepsize)
{
    int numberofsteps=round(100.0/stepsize);
    int t;
    double x;
    double y1, y1_prev;
    vec goods1(numberofsteps);
    vec bads1(numberofsteps);
    vec total1(numberofsteps);
    vec mbadrate1(numberofsteps);
    double approved_temp1;
    bool approved_temp_flag1;
    vec approved_flag1(numberofsteps);
    vec approved1(numberofsteps);
    double badrate_approved1=0;
    double approved_good1_temp;
    vec approved_good1(numberofsteps);
    double approved_bad1_temp;
    vec approved_bad1(numberofsteps);
    double approved_gini1;

    double y2, y2_prev;
    vec goods2(numberofsteps);
    vec bads2(numberofsteps);
    vec total2(numberofsteps);
    vec mbadrate2(numberofsteps);
    double approved_temp2;
    bool approved_temp_flag2;
    vec approved_flag2(numberofsteps);
    vec approved2(numberofsteps);
    double badrate_approved2=0;
    double approved_good2_temp;
    vec approved_good2(numberofsteps);
    double approved_bad2_temp;
    vec approved_bad2(numberofsteps);
    double approved_gini2;

    vec badratec3(numberofsteps);
    double badratec_good3;
    double badratec_bad3;
    double badrate_temp3;
    double approved_temp3;
    bool approved_temp_flag3;
    vec approved_flag3(numberofsteps);
    vec approved3(numberofsteps);
    double approved_good3_temp;
    vec approved_good3(numberofsteps);
    double approved_bad3_temp;
    vec approved_bad3(numberofsteps);
    double approved_gini3;
    double badrate_approved3=0;


    if(printdebug) {cout << "GINI 1: " << gini1 << endl;}
    if(printdebug) {cout << "GINI 2: " << gini2 << endl;}


    y1_prev=0;
    y2_prev=0;
    for (t = 0; t < numberofsteps; t++)
    {

        x = (t+1)*stepsize/100.0;
        y1 = beta*(pow(x, (1 - gini1) / (1 + gini1))) + (1 - beta)*(1 - pow((1 - x), (1 + gini1) / (1 - gini1)));
        y2 = beta*(pow(x, (1 - gini2) / (1 + gini2))) + (1 - beta)*(1 - pow((1 - x), (1 + gini2) / (1 - gini2)));
        if (t==numberofsteps-1 && workaround==true) {
                y1=1; y2=1;
                if (printdebug) {cout << x << " " << y1 << " " << y2 << " " << y1_prev << " " << y2_prev << endl;}
        }

        goods1[t]=(1-badrate)*stepsize/100.0;
        bads1[t]=badrate*(y1-y1_prev);
        total1[t]=goods1[t]+bads1[t];
        mbadrate1[t]=bads1[t]/total1[t];
        y1_prev=y1*1;
        goods2[t]=(1-badrate)*stepsize/100;
        bads2[t]=badrate*(y2-y2_prev);
        total2[t]=goods2[t]+bads2[t];
        mbadrate2[t]=bads2[t]/total2[t];
        y2_prev=y2*1;
	}

    approved_temp1=0;
    approved_temp_flag1=true;
    approved_temp2=0;
    approved_temp_flag2=true;

    for (t = numberofsteps-1; t >=0; t--)
    {
        if (approved_temp1+total1[t]<apprate && approved_temp_flag1==true) {
            approved1[t]=total1[t];
            approved_temp1=approved_temp1+total1[t];
            approved_flag1[t]=1;
        }
        else if (approved_temp1+total1[t]>=apprate && approved_temp_flag1==true) {
            approved_flag1[t]=(apprate-approved_temp1)/total1[t];
            approved1[t]=approved_flag1[t]*total1[t];
            approved_temp1=approved_temp1+approved1[t];
            approved_temp_flag1=false;
        }
        else if (approved_temp1+total1[t]>=apprate && approved_temp_flag1==false) {
            approved1[t]=0;
            approved_flag1[t]=0;
        }

        if (approved_temp2+total2[t]<apprate && approved_temp_flag2==true) {
            approved2[t]=total2[t];
            approved_temp2=approved_temp2+total2[t];
            approved_flag2[t]=1;
        }
        else if (approved_temp2+total2[t]>=apprate && approved_temp_flag2==true) {
            approved_flag2[t]=(apprate-approved_temp2)/total2[t];
            approved2[t]=approved_flag2[t]*total2[t];
            approved_temp2=approved_temp2+approved2[t];
            approved_temp_flag2=false;
        }
        else if (approved_temp2+total2[t]>=apprate && approved_temp_flag2==false) {
            approved2[t]=0;
            approved_flag2[t]=0;
        }

        badrate_approved1=badrate_approved1+mbadrate1[t]*approved1[t];
        badrate_approved2=badrate_approved2+mbadrate2[t]*approved2[t];
    }

    badrate_approved1=badrate_approved1/approved_temp1;
    badrate_approved2=badrate_approved2/approved_temp2;

//Third scenario calculation

	badratec_bad3=0;
    badratec_good3=0;
    badrate_temp3=0;
    approved_temp_flag3=true;
    approved_temp3=0;
    badrate_approved3=0;

    for (t = numberofsteps-1; t >=0; t--)
    {
        badratec_bad3=badratec_bad3+ bads2[t];
        badratec_good3=badratec_good3+goods2[t];
        badratec3[t]=badratec_bad3/(badratec_bad3+badratec_good3);

        if (badratec3[t]<badrate_approved1 && approved_temp_flag3==true) {
            approved3[t]=total2[t];
            badrate_temp3=badratec3[t];
            approved_temp3=approved_temp3+approved3[t];
            approved_flag3[t]=1;
        }
        else if (badratec3[t]>=badrate_approved1 && approved_temp_flag3==true) {
            approved_flag3[t]=(badrate_approved1-badrate_temp3)/(badratec3[t]-badrate_temp3);
            approved3[t]=approved_flag3[t]*total2[t];
            badrate_temp3=badratec3[t];
            approved_temp3=approved_temp3+approved3[t];
            approved_temp_flag3=false;
        }
        else if (badratec3[t]>=badrate_approved1 && approved_temp_flag3==false) {
            approved3[t]=0;
            approved_flag3[t]=0;
        }

        badrate_approved3=badrate_approved3+mbadrate2[t]*approved3[t];

   }

badrate_approved3=badrate_approved3/approved_temp3;


//GINI on approved calculated for the base scenario, scenario 1 and scenario 2

    approved_bad1_temp=0;
    approved_good1_temp=0;
    approved_gini1=0;
    approved_bad2_temp=0;
    approved_good2_temp=0;
    approved_gini2=0;
    approved_bad3_temp=0;
    approved_good3_temp=0;
    approved_gini3=0;

    for (t = 0; t < numberofsteps; t++)
    {
        approved_good1[t]=approved_good1_temp+goods1[t]*approved_flag1[t]/(1-badrate_approved1)/approved_temp1;
        approved_bad1[t]=approved_bad1_temp+bads1[t]*approved_flag1[t]/badrate_approved1/approved_temp1;
        approved_gini1=approved_gini1+(approved_good1[t]-approved_good1_temp)*(approved_bad1[t]+approved_bad1_temp)/2;
        approved_good1_temp=approved_good1[t];
        approved_bad1_temp=approved_bad1[t];

        approved_good2[t]=approved_good2_temp+goods2[t]*approved_flag2[t]/(1-badrate_approved2)/approved_temp2;
        approved_bad2[t]=approved_bad2_temp+bads2[t]*approved_flag2[t]/badrate_approved2/approved_temp2;
        approved_gini2=approved_gini2+(approved_good2[t]-approved_good2_temp)*(approved_bad2[t]+approved_bad2_temp)/2;
        approved_good2_temp=approved_good2[t];
        approved_bad2_temp=approved_bad2[t];

        approved_good3[t]=approved_good3_temp+goods2[t]*approved_flag3[t]/(1-badrate_approved3)/approved_temp3;
        approved_bad3[t]=approved_bad3_temp+bads2[t]*approved_flag3[t]/badrate_approved3/approved_temp3;
        approved_gini3=approved_gini3+(approved_good3[t]-approved_good3_temp)*(approved_bad3[t]+approved_bad3_temp)/2;
        approved_good3_temp=approved_good3[t];
        approved_bad3_temp=approved_bad3[t];
    }

    approved_gini1=approved_gini1*2-1;
    approved_gini2=approved_gini2*2-1;
    approved_gini3=approved_gini3*2-1;

//Printing the table to the console for debugging!

    if(printdebug) {
    for (t = numberofsteps-50; t < numberofsteps; t++)
    {
     cout << goods1[t] << "  " << bads1[t] << " " << total1[t] << " "
     << mbadrate1[t] << " " << approved_flag1[t] << " " << approved1[t] << " " << approved_good1[t] << " "
     << approved_bad1[t] << " " << endl;
    }


    cout << "Approval rate: " << approved_temp1 <<endl;
    cout << "Bad rate on approved: " << badrate_approved1 << endl;
    cout << "Approved GINI: " << approved_gini1 <<endl;
    cout << "Approval rate 2: " << approved_temp2 <<endl;
    cout << "Bad rate on approved 2: " << badrate_approved2 << endl;
    cout << "Approved GINI 2: " << approved_gini2 <<endl;
    cout << "Approval rate 3: " << approved_temp3 <<endl;
    cout << "Bad rate on approved 3: " << badrate_approved3 << endl;
    cout << "Approved GINI 3: " << approved_gini3 <<endl;
    }

    outp_badrate1=badrate_approved1;
    outp_badrate2=badrate_approved2;
    outp_badratereduction=(badrate_approved2-badrate_approved1)/badrate_approved1;
    outp_apprate3=approved_temp3;
    outp_approvalincrease=(approved_temp3-approved_temp1)/approved_temp1;
    outp_gini1=approved_gini1;
    outp_gini2=approved_gini2;
    outp_gini3=approved_gini3;


//Printing the two final results:

    if(printdebug) {
    cout << "Bad rate reduction: " << outp_badratereduction << endl;
    cout << "Approval increase: " << outp_approvalincrease <<endl;
    cout << "Number of steps: " << numberofsteps <<endl;
    }


}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

glPushMatrix();
glTranslatef(0,-.1,0);

	glPushMatrix();
        glTranslatef(-.95, -.8, 1);
        glScalef(.8, .8*w_width*1.0/w_height, 1);
        drawGraphArea();
        colorChartLine1(); drawGraph(inp_value[2]*1.0 / inp_gran[2], inp_value[0]*1.0 / inp_gran[0], 4);
        colorChartLine2(); drawGraph(inp_value[2]*1.0 / inp_gran[2], inp_value[1]*1.0 / inp_gran[1], 4);
	glPopMatrix();

    glLineWidth(1);
	colorInputSelection();
	drawInputSelection();
	colorInputArea();
    drawInputArea();
//void recalculate main(float gini1, float gini2, float beta, float badrate, float apprate, float stepsize)
    if(printdebug) {
    cout << "GINI value 1: " << inp_value[0] << endl;
    cout << "GINI value 2: " << inp_value[1] << endl;
    cout << "GINI gran 1: " << inp_gran[0] << endl;
    cout << "GINI 1: " << inp_value[0]*1.0/inp_gran[0] << endl;
    }

    if (inputbox_flag==false)
    {recalculate(inp_value[0]*1.0/inp_gran[0], inp_value[1]*1.0/inp_gran[1], inp_value[2]*1.0/inp_gran[2], inp_value[3]*1.0/inp_gran[3], inp_value[4]*1.0/inp_gran[4], numsteps[inp_value[5]]);}
    drawOutputArea();
    if (inputbox_flag==true && inp_numstep[inp_flag]==false) {colorInputSelection(); drawInputBox();};
	colorHelpArea();
	drawHelpArea();



glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(w_width, w_height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Scoringator");
	glutDisplayFunc(display);
	glutSpecialFunc(handleSpecialpress);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(handleMouseclick);
	glutPassiveMotionFunc(handleMouseover);
	glutReshapeFunc(handleReshape);
	glutMainLoop();
	return 0;
}
