
/*******************************************|
 COURSE PROJECT PART 2 ENSC 488
 |*******************************************/

#ifdef _WIN32
#include <GL/glut.h>
#include "glui.h"
#elif __APPLE__
#include <GLUT/glut.h>
#include <GLUI/glui.h>
#elif __LINUX__
#include <GL/glut.h>
#endif

#include <algorithm>

#include <unistd.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <time.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string.h>

#include <iostream>
#include <stdio.h> // Standard Input and Output Library

#include "objects.h"

using namespace std;

//GLUI Variables
int mainWindow = 0;
GLUI * subWindow = 0;
char goalxchar[8], goalzchar[8], via1xchar[8], via1zchar[8], via2xchar[8], via2zchar[8];
char theta1char[8], theta2char[8], theta3char[8], theta4char[8], theta5char[8], theta6char[8];
char obsheightchar[8], masschar[8];
int goalx = 0, goalz = 0;
int via1x = 0, via1z = 0, via2x = 0, via2z = 0;
double obsheight = 0, mass = 0, theta = 0;
int mainw, gridw, perspw;
int goalselection=0, initialnode=0, tempnode=0;

// angles = { Y rotation at shoulder, Z rotation at shoulder, Z rotation at elbow, X rotation at wrist }
double arm_angles[6] = {45, -10, 70, 0, 20, 0}; //INITIAL CONDITIONS AT TOP MAX

double theta1,theta2,theta3,theta4,theta5,theta6;

GLfloat cameraDistance = 12, cameraAngle = 0;

// Variables for planar movement of the platform
int cz = 1;
int cx = 1;
int cy = 0;
bool turn = false;
bool zdirection = false, xdirection = false;
int tempx=0,tempz=0,wheelturn=0;
int pathpoints[3][2]={{0}};

int obs_check[17][17] = {{0}};

//GLUI
void setupGlui();
void gluiCallback(int id);
void MoveRobot();

// **************** Functions
void display(void);
void init(void);
void makeRobot(int);
void change_angle(int, int, double, double);

//------------GLUI MENU
void setupGlui()
{
	subWindow = GLUI_Master.create_glui("Control Window", 0, 600, 0);
	GLUI_Panel *TOP = subWindow->add_panel("");
	
	subWindow->add_column_to_panel(TOP);
	
	GLUI_Panel *kinematics = subWindow->add_panel_to_panel(TOP, "Arm Angles (FK)");
	subWindow->add_edittext_to_panel(kinematics,"Base",GLUI_EDITTEXT_TEXT,theta1char);
	subWindow->add_edittext_to_panel(kinematics,"Shoulder",GLUI_EDITTEXT_TEXT,theta2char);
	subWindow->add_edittext_to_panel(kinematics,"Elbow",GLUI_EDITTEXT_TEXT,theta3char);
	subWindow->add_edittext_to_panel(kinematics,"Wrist",GLUI_EDITTEXT_TEXT,theta4char);
	subWindow->add_edittext_to_panel(kinematics,"Pencil Z",GLUI_EDITTEXT_TEXT,theta5char);
	subWindow->add_edittext_to_panel(kinematics,"Pencil X",GLUI_EDITTEXT_TEXT,theta6char);
	
	GLUI_Panel *button_panel2 = subWindow->add_panel_to_panel(TOP, "ACTIONS");
	subWindow->add_button_to_panel(button_panel2,"Move Arm", 0, gluiCallback);
	subWindow->add_button_to_panel(button_panel2,"Quit", 1, gluiCallback);

	subWindow->add_column();
	GLUI_Panel *inverse = subWindow->add_panel_to_panel(TOP,"Reach Points");
	subWindow->add_button_to_panel(inverse,"Reach Highest", 2, gluiCallback);
	subWindow->add_button_to_panel(inverse,"Reach Mid", 3, gluiCallback);
	subWindow->add_button_to_panel(inverse,"Reach Lowest", 4, gluiCallback);
	
	subWindow->set_main_gfx_window(mainWindow);
	subWindow->sync_live();
}

void gluiCallback(int id)
{
	switch(id)
	{
		case 0:
			theta1=atof(theta1char);
			change_angle(BASE_X, theta1,0,180);
			theta2=atof(theta2char);
			change_angle(SHOULDER_Z, theta2, -10, 90);
			theta3=atof(theta3char);
			change_angle(ELBOW_Z,theta3,70,135);
			theta4=atof(theta4char);
			change_angle(WRIST_X, theta4,0,360);
			theta5=atof(theta5char);
			change_angle(PENCIL_Z, theta5, 20, 180);
			theta6=atof(theta6char);
			display();
			glutSetWindow(mainw);
			glutSwapBuffers();
			break;
		case 1:
			exit(1);
			break;
		case 2: //reach highest
			theta1=atof(theta1char);
			change_angle(BASE_X, 0,0,180);
			theta2=atof(theta2char);
			change_angle(SHOULDER_Z, 0, -10, 90);
			theta3=atof(theta3char);
			change_angle(ELBOW_Z,0,70,135);
			theta4=atof(theta4char);
			change_angle(WRIST_X, 0,0,360);
			theta5=atof(theta5char);
			change_angle(PENCIL_Z, 0, 20, 180);
			theta6=atof(theta6char);
			display();
			glutSetWindow(mainw);
			glutSwapBuffers();
			break;
		case 3: //reach mid
			theta1=atof(theta1char);
			change_angle(BASE_X, 0,0,180);
			theta2=atof(theta2char);
			change_angle(SHOULDER_Z, 45, -10, 90);
			theta3=atof(theta3char);
			change_angle(ELBOW_Z,0,70,135);
			theta4=atof(theta4char);
			change_angle(WRIST_X, 0,0,360);
			theta5=atof(theta5char);
			change_angle(PENCIL_Z, 0, 20, 180);
			theta6=atof(theta6char);
			display();
			glutSetWindow(mainw);
			glutSwapBuffers();
			break;
		case 4: //reach lowest
			theta1=atof(theta1char);
			change_angle(BASE_X, 0,0,180);
			theta2=atof(theta2char);
			change_angle(SHOULDER_Z, 90, -10, 90);
			theta3=atof(theta3char);
			change_angle(ELBOW_Z,0,70,135);
			theta4=atof(theta4char);
			change_angle(WRIST_X, 0,0,360);
			theta5=atof(theta5char);
			change_angle(PENCIL_Z, 0, 20, 180);
			theta6=atof(theta6char);
			display();
			glutSetWindow(mainw);
			glutSwapBuffers();
			break;
	}
	glutPostRedisplay();
}


double node[6][2] = { {3,7},{3,11},{6,9},{11,6},{7,2},{12,3} };
//FOR MOVE ROBOT
void increasez(){
	if(++cz>15){
		cz=15;
	}
}
void decreasez(){
	if(--cz<1){
		cz=1;
	}
}

void moveZ(int x, int i){
	
	if(cz < pathpoints[i][1]){
		for(int z = cz; z < pathpoints[i][1]; z++){
			if(obs_check[cz+2][cx]!=0 || obs_check[cz+2][cx+1]!=0){ //check two blocks in front
				cout<<"Bumped into object Case3"<<endl;
				continue;
			}else{
				turn = true;
				zdirection = false;
				xdirection = false;
				increasez();
				display();
				usleep(200000);
				cout<<"CZ: "<<cz<<endl;
			}
		}
	}
	else if(cz>pathpoints[i][1]){
		for(int z = cz; z > pathpoints[i][1]; z--){
			if(obs_check[cz-1][cx]!=0 || obs_check[cz-1][cx+1]!=0){
				cout<<"Bumped into object Case4"<<endl;
				continue;
			}else{
				turn = true;
				zdirection = true;
				xdirection = false;
				decreasez();
				display();
				usleep(200000);
				cout<<"CZ: "<<cz<<endl;
			}
		}
	}
}

void moveX(int i){
	if(cx<pathpoints[i][0]){
		for(int x = cx; x < pathpoints[i][0]; x++){
			moveZ(x,i);
			if(obs_check[cz][cx+2]!=0 || obs_check[cz+1][cx+2]!=0){ //CHECK FRONT
				cout<<"Bumped into object Case1"<<endl;
				continue;
			}else{
				turn = false;
				zdirection = false;
				xdirection = false;
				cx++;
				display();
				usleep(200000);
				cout<<"CX: "<<cx<<endl;
			}
		}
	}
	else if(cx>pathpoints[i][0]){
		for(int x = cx; x > pathpoints[i][0]; x--){
			moveZ(x,i);
			if(obs_check[cz][cx-1]!=0 || obs_check[cz+1][cx-1]!=0){
				cout<<"Bumped into object Case2"<<endl;
				continue;
			}else{
				turn = false;
				zdirection = false;
				xdirection = true;
				cx--;
				display();
				usleep(200000);
				cout<<"CX: "<<cx<<endl;
			}
		}
	}
	else{
		moveZ(cx,i);
		display();
		usleep(200000);
		cout<<"CX: "<<cx<<endl;
	}
}

void MoveRobot(){
	glutSetWindow(mainw);
	goalx = node[goalselection][0]; //define goal point
	goalz = node[goalselection][1];
	cout<<"GOAL: "<< goalx<<", "<<goalz<<endl;
	
	via1x = atof(via1xchar);
	via1z = atof(via1zchar);
	
	via2x = atof(via2xchar);
	via2z = atof(via2zchar);
	
	pathpoints[0][0]=via1x;
	pathpoints[0][1]=via1z;
	
	pathpoints[1][0]=via2x;
	pathpoints[1][1]=via2z;
	
	pathpoints[2][0]=goalx;
	pathpoints[2][1]=goalz;
	
	cout<<"VIA 1: "<<via1x<<", "<<via1z<<endl;
	cout<<"VIA 2: "<<via2x<<", "<<via2z<<endl;
	
	for(int i=0;i<3;i++){
		if(pathpoints[i][0] == 0 && pathpoints[i][1] == 0){
			continue; //VIA IS ZERO, NOT SPECIFIED, IGNORE
		}
		moveX(i); //start moving
		if(cx==goalx && cz != goalz){ //bumped into something, maybe moving in Z again can still reach goal
			moveZ(cz,2);
			display();
			usleep(1000000);
		}
		if(cz==goalz && cx != goalx){ //bumped into something, maybe moving in X again can still reach goal
			moveX(2);
			display();
			usleep(1000000);
		}
		cout<<"Move Done"<<endl;
	}
	
}

void change_angle(int angle, int delta, double minimum = 0, double maximum = 180) {
	arm_angles[angle] = (delta) % 360;
	arm_angles[angle] = max(arm_angles[angle], minimum);
	arm_angles[angle] = min(arm_angles[angle], maximum);
}

void makeRobot(int rotate)
{
	glTranslatef(cx, 0, cz);
	
	glTranslated(0, 0, -0.2);
	glRotated(90, 0, 0, 1); // rotate so the arm is upright
	
	glPushMatrix();
	
	glRotated(rotate, 1, 0, 0);
	
	if (turn == false){ //moving on X axis
		tempx = cx % 8; // In this case there are 8 steps needed
						// The number of degrees rotated in each step
		wheelturn = -45 * tempx; // Since there are 8 steps -> 360/8 = 45 degrees
	}
	else{ //moving on z axis
		tempz = cz % 8; // In this case there are 8 steps needed
						// The number of degrees rotated in each step
		wheelturn = -45 * tempz; // Since there are 8 steps -> 360/8 = 45 degrees
	}
	
	glTranslated(1, 0, 0);
	glScaled(0.7, 0.7, 0.7);
	glRotated(135,1,0,0);
	makeArm();
	
	glPopMatrix();
	
	glRotated(-90, 0,0,1 );
	glTranslatef(-cx, 0, -cz);
	
}


// Function for keyboard entry to manipulate the haptic device
void keyboard(unsigned char key, int x, int y)
{
	
	switch (key)
	{
			// Exit case by hitting 'ESC' key
		case 27: //ESC in ASCII
			exit(1);
			break;
	}
	glutPostRedisplay();
}



// Now the fun part, putting everything together
void drawHouse()
{
	glLoadIdentity();
	int CamX=cx + cos(20)*6;
	int CamY=4;
	int CamZ=cz + sin(20)*6;
	gluLookAt(CamX, CamY, CamZ, cx, 2, cz, 0, 1, 0);
	
	// Draw a 15 x 15 x 15 room
	drawWall(0, 0, 15, 15);
	
	drawFloor();
	drawGrid(7.5);
	glColor3f(1, 1, 1);
	//	makeRug(); // Rug function needs to be called first so that the furniture can be places on top of it
	
	//HOME BASE
	glColor3d(1, 1, 0);
	drawSquarePrism(0, 0, 0, 2);
	
	makeTable();
	makeShelf();
	makeChair();
	makeTV();
	
	drawHumanoid(0, 0, 14); //Standing left of Char1
	obs_check[15][1]=1;
	drawHumanoid(0, 0, 10); // Standing between the chairs
	obs_check[11][1]=1;
	drawHumanoid(9, 0, 1); // Standing between TV and bookshelf
	obs_check[2][10]=1;
	
	if(turn==true && zdirection == true){
		makeRobot(-90);
	}else if(turn == true && zdirection == false){
		makeRobot(90);
	}else if(xdirection == true){
		makeRobot(180);
	}else{
		makeRobot(0);
	}

	return;
}

// Main Display Function
void display()
{
	glutSetWindow(mainw);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Display the masterpiece on the screen
	drawHouse();
	glutPostRedisplay();
	glFlush();
	
	return;
}

void init()
{
	glutSetWindow(mainw);
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Reset View
	gluPerspective(90.0, 16 / 9, 1, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	return;
	
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	//CAMERA WINDOW
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 400); // Size the window
	glutCreateWindow("ROBOTICS PROJECT");
	
	glutDisplayFunc(display);
	init();
	mainw = glutGetWindow();
	
	glutIdleFunc(display);

	glutKeyboardFunc(keyboard);
	
	setupGlui();
	glutMainLoop();
	
	return 0;
}
