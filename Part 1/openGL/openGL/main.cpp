
/*******************************************|
 COURSE PROJECT PART 1 ENSC 488
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
int goalx = 0, goalz = 0;
int via1x = 0, via1z = 0, via2x = 0, via2z = 0;
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
int tempx=0,tempz=0,wheelturn=0;
int pathpoints[3][2]={{0}};

int dist[7]={0};
int prevn[7]={0};

int obs_check[17][17] = {{0}};

//GLUI
void setupGlui();
void gluiCallback(int id);
void MoveRobot();

// **************** Functions
void display(void);
void display2(void);
void display3(void);
void init(void);
void makeRobot(int);
void change_angle(int, int, double, double);


//------------GLUI MENU
void setupGlui()
{
	subWindow = GLUI_Master.create_glui("Control Window", 0, 1000, 0);
	GLUI_Panel *goal= subWindow->add_panel ("Goal Position");
	
	GLUI_RadioGroup *goalradio = subWindow->add_radiogroup_to_panel(goal, &tempnode, 4, gluiCallback);
	subWindow->add_radiobutton_to_group(goalradio, "First Chair" );
	subWindow->add_radiobutton_to_group(goalradio, "Second Chair" );
	subWindow->add_radiobutton_to_group(goalradio, "Table Left" );
	subWindow->add_radiobutton_to_group(goalradio, "Table Right" );
	subWindow->add_radiobutton_to_group(goalradio, "TV Table" );
	subWindow->add_radiobutton_to_group(goalradio, "Shelf" );
	
	GLUI_Panel *via_panel = subWindow->add_panel("VIA (optional)");
	subWindow->add_edittext_to_panel(via_panel,"ViaX 1", GLUI_EDITTEXT_TEXT, via1xchar);
	subWindow->add_edittext_to_panel(via_panel,"ViaZ 1", GLUI_EDITTEXT_TEXT, via1zchar);
	
	subWindow->add_edittext_to_panel(via_panel,"ViaX 2", GLUI_EDITTEXT_TEXT, via2xchar);
	subWindow->add_edittext_to_panel(via_panel,"ViaZ 2", GLUI_EDITTEXT_TEXT, via2zchar);
	
	GLUI_Panel *button_panel = subWindow->add_panel("ACTIONS");
	subWindow->add_button_to_panel(button_panel,"Move Robot", 0, gluiCallback);
	subWindow->add_button_to_panel(button_panel,"Go Home", 1, gluiCallback);
	subWindow->add_button_to_panel(button_panel,"QUIT", 2, gluiCallback);
	
	subWindow->set_main_gfx_window(mainWindow);
	subWindow->sync_live();
}

void gluiCallback(int id)
{
	switch(id)
	{
		case 0:
			cout<<"Moving Robot..."<<endl;
			initialnode = goalselection;
			goalselection = tempnode;
			cout<<"*****"<<initialnode<<", "<<goalselection<<endl;
			glutSetWindow(mainw);
			printobs_check();
			MoveRobot();
			break;
		case 1:
			cx = 1;
			cz = 1;
			initialnode=0;
			turn=false;
			display();
			glutSetWindow(mainw);
			glutSwapBuffers();
			break;
		case 2:
			exit(1);
			break;
		case 3:
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
				increasez();
				display();
				display2();
				display3();
				usleep(1000000);
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
				decreasez();
				display();
				display2();
				display3();
				usleep(1000000);
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
				cx++;
				display();
				display2();
				display3();
				usleep(1000000);
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
				cx--;
				display();
				display2();
				display3();
				usleep(1000000);
				cout<<"CX: "<<cx<<endl;
			}
		}
	}
	else{
		moveZ(cx,i);
		display();
		usleep(1000000);
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
	
	if(rotate == -90){
		makeWheel2();
		makeWheel3();
		makeWheel1();
		makeWheel4();
		makePlatform();
	}else{
		makeWheel4();
		makeWheel1();
		makePlatform();
		makeWheel2();
		makeWheel3();
	}
	makeCoordinateFrame();
	glPopMatrix();
	
	glRotated(-90, 0,0,1 );
	glTranslatef(-cx, 0, -cz);
	
}

// Special keys to move the robot around manually
void specialKeys(int key, int x, int y)
{
	//MANUAL MOVE OVERRIDE

	// Up arrow key is pressed
	if (key == GLUT_KEY_UP)
	{
		turn = true;
		cz++;
	}
	
	// Down arrow key is pressed
	if (key == GLUT_KEY_DOWN)
	{
		turn = true;
		cz--;
	}
	
	// Left arrow key is pressed
	if (key == GLUT_KEY_LEFT)
	{
		turn = false;
		cx--;
		
	}
	
	// Right arrow key is pressed
	if (key == GLUT_KEY_RIGHT)
	{
		turn = false;
		cx++;
	}
	cout<<"CX: "<<cx<<", CZ: "<<cz<<endl;
	glutPostRedisplay();
}


// Function for keyboard entry to manipulate the haptic device
void keyboard(unsigned char key, int x, int y)
{
	int delta = 5;
	
	switch (key)
	{
			// Exit case by hitting 'ESC' key
		case 27: //ESC in ASCII
			exit(1);
			break;
			//			// The Base is rotated using ASWD
			//			// Move the BASE_X joint in + & - diections
			//		case 'a':
			//			change_angle(BASE_X, delta, -180, 0);
			//			break;
			//		case 'd':
			//			change_angle(BASE_X, -delta, -180, 0);
			//			break;
			//			// Move the SHOULDER_Z joint in + & - diections
			//		case 'n':
			//			change_angle(SHOULDER_Z, delta, 0, 135);
			//			break;
			//		case 'm':
			//			change_angle(SHOULDER_Z, -delta, 0, 135);
			//			break;
			//
			//			// Move the ELBOW_Z joint in + & - diections
			//		case 'j':
			//			change_angle(ELBOW_Z, delta, 0, 135);
			//			break;
			//		case 'k':
			//			change_angle(ELBOW_Z, -delta, 0, 135);
			//			break;
			//
			//			// Move the WRIST_X joint in + & - diections
			//		case 'p':
			//			change_angle(WRIST_X, delta, -45, 45);
			//			break;
			//		case 'l':
			//			change_angle(WRIST_X, -delta, -45, 45);
			//			break;
			//
			//			// Move the WRIST_X joint in + & - diections
			//		case 'i':
			//			change_angle(WRIST_X, delta, -15, 90);
			//			break;
			//		case 'o':
			//			change_angle(WRIST_X, -delta, -15, 90);
			//			break;
	}
	glutPostRedisplay();
}



// Now the fun part, putting everything together
void drawHouse()
{
	glLoadIdentity();
	int CamX=cx + cos(20)*6;
	int CamY=5;
	int CamZ=cz + sin(20)*6;
	gluLookAt(CamX, CamY, CamZ, cx, 2, cz, 0, 1, 0);
	
	// Draw a 15 x 15 x 15 room
	drawWall(0, 0, 15, 15);
	//viaPoint(viax, viaz);
	
	// haptic();
	// Put the furniture in the room
	
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
	
	if(turn==true){
		makeRobot(-90);
	}else{
		makeRobot(0);
	}
	
	return;
}

void drawmapgrid(){
	
	// Draw a 15 x 15 x 15 room
	drawWall(0, 0, 15, 15);
	
	drawFloor();
	drawGrid(7.5);
	glColor3f(1, 1, 1);
	
	//Draw stations
	for(int i=0;i<6;i++){
		glColor3d(0.8, 1, 0);
		drawSquarePrism(node[i][0], node[i][1], 0, 1);
	}
	
	//HOME BASE
	glColor3d(1, 1, 0);
	drawSquarePrism(0, 0, 0, 2);
	
	makeTable();
	makeShelf();
	makeChair();
	makeTV();
	
	drawHumanoid(0, 0, 14); //Standing left of Char1
	drawHumanoid(0, 0, 10); // Standing between the chairs
	drawHumanoid(9, 0, 1); // Standing between TV and bookshelf
	
	if(turn==true){
		makeRobot(-90);
	}else{
		makeRobot(0);
	}
	
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

void display2(){ //PERSPECTIVE VIEW
	glutSetWindow(perspw);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(20, 10, 20, 0, 0, 0, 0, 1, 0);
	drawmapgrid();
	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
	
	return;
}

void display3(){ //TOP VIEW
	glutSetWindow(gridw);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, 1);
	
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glTranslated(-6, -3, -4);
	drawmapgrid();
	glPopMatrix();
	
	glutPostRedisplay();
	glutSwapBuffers();
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

void init2()
{
	glutSetWindow(perspw);
	// Set the current clear color to black and the current drawing color to white.
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Reset View
	gluPerspective(90.0, 16 / 9, 1, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return;
	
}

void init3(){
	glutSetWindow(gridw);
	// Set the current clear color to black and the current drawing color to white.
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
	
	//PERSPECTIVE WINDOW
	glutInitWindowPosition(600, 0);
	glutInitWindowSize(400, 300); // Size the window
	glutCreateWindow("PERSPECTIVE");
	
	perspw = glutGetWindow();
	
	glutDisplayFunc(display2);
	init2();
	
	//GRID WINDOW
	glutInitWindowPosition(600, 350);
	glutInitWindowSize(400, 300); // Size the window
	glutCreateWindow("GRID MAP");
	
	gridw = glutGetWindow();
	
	glutDisplayFunc(display3);
	init3();
	
	//CAMERA WINDOW
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 400); // Size the window
	glutCreateWindow("ROBOTICS PROJECT");
	
	glutDisplayFunc(display);
	init();
	mainw = glutGetWindow();

	glutIdleFunc(display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	
	setupGlui();
	glutMainLoop();
	
	return 0;
}
