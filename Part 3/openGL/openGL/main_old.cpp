//
///*******************************************|
// COURSE PROJECT ENSC 488
// |*******************************************/
//
//#ifdef _WIN32
//#include <GL/glut.h>
//#include "glui.h"
//#elif __APPLE__
//#include <GLUT/glut.h>
//#include <GLUI/glui.h>
//#elif __LINUX__
//#include <GL/glut.h>
//#endif
//
//#include <algorithm>
//
//#include <unistd.h>
//#include <math.h>
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <cmath>
//#include <time.h>
//#include <iostream>
//#include <cstdio>
//#include <cstring>
//#include <string.h>
//
//#include <iostream>
//#include <stdio.h> // Standard Input and Output Library
//
//#include "objects.h"
//
//using namespace std;
//
////GLUI Variables
//int mainWindow = 0;
//GLUI * subWindow = 0;
//char goalxchar[8], goalzchar[8], viaxchar[8], viazchar[8];
//char theta1char[8], theta2char[8], theta3char[8], theta4char[8], theta5char[8], theta6char[8];
//double goalx = 0, goalz = 0;
//double viax = 0, viaz = 0;
//int mainw;
//int goalselection=0, initialnode=0, tempnode=0;
//
//// angles = { Y rotation at shoulder, Z rotation at shoulder, Z rotation at elbow, X rotation at wrist }
//double arm_angles[6] = {45, -10, 70, 0, 20, 0}; //INITIAL CONDITIONS AT TOP MAX
//
//double theta1,theta2,theta3,theta4,theta5,theta6;
//
//GLfloat cameraDistance = 12, cameraAngle = 0;
//
//// Variables for planar movement of the platform
//int cz = 12.5;
//int cx = 2.5;
//int cy = 0;
//int dist[7]={0};
//int prevn[7]={0};
//
//vector<int> vec;
//int obs_check[14][14] = {{0}};
//
////GLUI
//void setupGlui();
//void gluiCallback(int id);
//void MoveRobot();
//
//// Functions
//void display(void);
//void init(void);
//void makeRobot();
//void change_angle(int, int, double, double);
//
////------------GLUI MENU
//void setupGlui()
//{
//	subWindow = GLUI_Master.create_glui("Control Window", 0, 800, 150);
//	GLUI_Panel *goal= subWindow->add_panel ("Goal Position");
//	
//	GLUI_RadioGroup *goalradio = subWindow->add_radiogroup_to_panel(goal, &tempnode, 4, gluiCallback);
//	subWindow->add_radiobutton_to_group(goalradio, "Bottom Chair" );
//	subWindow->add_radiobutton_to_group(goalradio, "Chair" );
//	subWindow->add_radiobutton_to_group(goalradio, "Table Left" );
//	subWindow->add_radiobutton_to_group(goalradio, "Table Right" );
//	subWindow->add_radiobutton_to_group(goalradio, "TV Table" );
//	subWindow->add_radiobutton_to_group(goalradio, "Shelf" );
//	
//	GLUI_Panel *button_panel = subWindow->add_panel("ACTIONS");
//	subWindow->add_button_to_panel(button_panel,"Move Robot", 0, gluiCallback);
//	subWindow->add_button_to_panel(button_panel,"Go Home", 1, gluiCallback);
//	subWindow->add_button_to_panel(button_panel,"QUIT", 2, gluiCallback);
//	
//	subWindow->add_column();
//	
//	GLUI_Panel *kinematics = subWindow->add_panel ("Arm Angles (FK)");
//	subWindow->add_edittext_to_panel(kinematics,"Base",GLUI_EDITTEXT_TEXT,theta1char);
//	subWindow->add_edittext_to_panel(kinematics,"Shoulder",GLUI_EDITTEXT_TEXT,theta2char);
//	subWindow->add_edittext_to_panel(kinematics,"Elbow",GLUI_EDITTEXT_TEXT,theta3char);
//	subWindow->add_edittext_to_panel(kinematics,"Wrist",GLUI_EDITTEXT_TEXT,theta4char);
//	subWindow->add_edittext_to_panel(kinematics,"Pencil Z",GLUI_EDITTEXT_TEXT,theta5char);
//	subWindow->add_edittext_to_panel(kinematics,"Pencil X",GLUI_EDITTEXT_TEXT,theta6char);
//	
//	GLUI_Panel *button_panel2 = subWindow->add_panel("ACTIONS");
//	subWindow->add_button_to_panel(button_panel2,"Move Arm", 3, gluiCallback);
//	
//	subWindow->set_main_gfx_window(mainWindow);
//	subWindow->sync_live();
//}
//
//void gluiCallback(int id)
//{
//	switch(id)
//	{
//		case 0:
//			cout<<"Moving Robot..."<<endl;
//			initialnode = goalselection;
//			goalselection = tempnode;
//			cout<<"*****"<<initialnode<<endl;
//			cout<<goalselection<<endl;
//			MoveRobot();
//			break;
//		case 1:
//			cx = 2.5;
//			cz = 12.5;
//			initialnode=0;
//			display();
//			glutSetWindow(mainw);
//			glutSwapBuffers();
//			break;
//		case 2:
//			exit(1);
//			break;
//		case 3:
//			theta1=atof(theta1char);
//			change_angle(BASE_X, theta1,0,180);
//			theta2=atof(theta2char);
//			change_angle(SHOULDER_Z, theta2, -10, 90);
//			theta3=atof(theta3char);
//			change_angle(ELBOW_Z,theta3,70,135);
//			theta4=atof(theta4char);
//			change_angle(WRIST_X, theta4,0,360);
//			theta5=atof(theta5char);
//			change_angle(PENCIL_Z, theta5, 20, 180);
//			theta6=atof(theta6char);
//			display();
//			glutSetWindow(mainw);
//			glutSwapBuffers();
//			break;
//	}
//	glutPostRedisplay();
//}
//
//// Dijkstra Algorithm to find the shortest moving path
//int maxint = 9999999;  //which means the node is not reachable
//					   //node 1(bottom chair) at(2.5,12.5)
//					   //node 2(chair) at(2.5,8.5)
//					   //node 3(left top of table)at(3.5,3.5)
//					   //node 4(right bottom of table)at(7.5,6.5)
//					   //node 5(table under TV)at(7,1.5)
//					   //node 6(shelf)at(12,2.5)
//double node[6][2] = { {2.5,12.5},{2.5,8.5},{3.5,3.5},{7.5,6.5},{7,1.5},{12,2.5} };
//double c[7][7] = { { 0,0,0,0,0,0,0},{ 0,0,4,9,7.8,9999999,13.4},{ 0,4,0,5,5.4,9999999,9999999 },{ 0,9,5,0,9999999,4,8.6 },{ 0,7.8,5.4,9999999,0,5,6 },{ 0,9999999,9999999,4,5,0,5 },{ 0,13.4,9999999,8.6,6,5,0 } };
//bool s[7];
//
//void Dijkstra(int n, int v, int dist[7], int prev[7]) {// n is the number of nodes and v represents the current node
//	for (int i = 1; i <= n; i++) {
//		dist[i] = c[v][i];//initialize the distance between current node to node i
//		s[i] = false;//
//		if (dist[i] == maxint)prev[i] = 0;//to check if the path from v to i is a direct path; if not, assign the previous node to prev[]
//		else prev[i] = v;
//	}
//	dist[v] = 0; s[v] = true;//set v to be true
//	for (int i = 1; i <= n; i++) {
//		int temp = maxint;
//		int u = v;
//		for (int j = 1; j <= n; j++) {
//			if ((!s[j]) && (dist[j]<temp)) {//find the shortest node without in s[]
//				u = j;
//				temp = dist[j];
//			}
//		}
//		s[u] = true;// put the new node into s[]
//		for (int j = 1; j <= n; j++) {//set node u as a new current node, and renew the data in dist[]
//			if ((!s[j]) && (c[u][j]<maxint)) {//find the nodes that make direct path from u to j, and j is not in s[]
//				int newdist = dist[u] + c[u][j];
//				if (newdist<dist[j]) {//if the new path from u to j is shorter than previous path, renew the data in dist[]
//					dist[j] = newdist;
//					prev[j] = u;
//				}
//			}
//		}
//	}
//}
//void foundDist(int dist, int prev[]) {
//	int prevNode = prev[dist];
//	vec.push_back(dist);
//	vec.push_back(prevNode);
//	prevNode = prev[prevNode];
//	vec.push_back(prevNode);
//}
//
//void FindShortest()
//{
//	Dijkstra(6, initialnode+1, dist, prevn);
//	foundDist(goalselection+1, prevn);
//}
//
//void MoveRobot(){
//	//	FindShortest();
//	goalx = node[goalselection][0]; //define goal point
//	goalz = node[goalselection][1];
//	cout<<"GOAL: "<< goalx<<", "<<goalz<<endl;
//	if(vec.size()<3){ //Assign same end point
//		cout<<"Vector size:"<<vec.size()<<endl;
//		viax = node[goalselection][0];
//		viaz = node[goalselection][1];
//	}
//	
//	else{ //Assign a mid point
//		viax = node[vec[1]][0];
//		viaz = node[vec[1]][1];
//	}
//	cout<<"VIA: "<<viax<<", "<<viaz<<endl;
//	
//	if (obs_check[(int)viaz][(int)viax] != 0)
//	{
//		cout << "There is an obstacle." << endl;
//	}
//	
//	else
//	{
//		for (int i = 0; i < viax; i++)
//		{
//			if(viax>(int)cx){
//				cx++;
//			}else{
//				cx--;
//			}
//			cout<<"CX: "<<cx<<endl;
//			init();
//			display();
//			usleep(1000000);
//			glutSetWindow(mainw);
//			glutSwapBuffers();
//		}
//		
//		for (int j = 0; j < viaz; j++)
//		{
//			if(viaz>(int)cz){
//				cz++;
//			}else{
//				cz--;
//			}
//			cout<<"CZ: "<<cz<<endl;
//			init();
//			display();
//			usleep(1000000);
//			glutSetWindow(mainw);
//			glutSwapBuffers();
//			
//		}
//		
//	}
//	
//	cout<<"Via point reached"<<endl;
//	usleep(2000000);
//	
//	int greaterx = 0;
//	
//	if (viax > goalx)
//	{
//		greaterx = 1;
//	}
//	else
//	{
//		greaterx = 0;
//	}
//	
//	
//	for (int i = 0; i < abs(goalx-viax); i++)
//	{
//		if (greaterx > 0)
//		{
//			cx-=0.5;
//		}
//		else
//		{
//			cx+=0.5;
//		}
//		init();
//		display();
//		usleep(1000000);
//		glutSetWindow(mainw);
//		glutSwapBuffers();
//		
//	}
//	
//	int greaterz = 0;
//	
//	if (viaz > goalz)
//	{
//		greaterz = 1;
//	}
//	else
//	{
//		greaterz = 0;
//	}
//	
//	for (int j = 0; j < abs(goalz - viaz); j++)
//	{
//		if (greaterz > 0)
//		{
//			cz-=0.5;
//			if (obs_check[cz + 1][cx + 1] != 0)
//			{
//				cout << "There is an obstacle" << endl;
//			}
//		}
//		else
//		{
//			cz+=0.5;
//		}
//		init();
//		display();
//		usleep(1000000);
//		glutSetWindow(mainw);
//		glutSwapBuffers();
//		
//	}
//	cout << "You have arrived at your destination!" << endl;
//	init();
//}
//
//void change_angle(int angle, int delta, double minimum = 0, double maximum = 180) {
//	arm_angles[angle] = (delta) % 360;
//	arm_angles[angle] = max(arm_angles[angle], minimum);
//	arm_angles[angle] = min(arm_angles[angle], maximum);
//}
//
//void makeRobot(void)
//{
//	
//	glScalef(0.5, 0.5, 0.5); // scale to make the base smaller
//							 //glClear(GL_COLOR_BUFFER_BIT);
//							 // Specify the vewing angles/distance
//	glLoadIdentity();
//	gluLookAt(20, 10, 20, 0, 0, 0, 0, 1, 0);
//	
//	glTranslatef(cx, 0, cz);
//	glRotated(90, 0,0,1 ); // rotate so the arm is upright
//	
//	// Make Platform
//	glPushMatrix();
//	glColor3f(0, 1, 0);
//	glTranslatef(-1.5, 0, 0);
//	glScalef(0.25, 1, 1);
//	glutSolidCube(1.5);
//	glPopMatrix();
//	
//	// Make Wheel 1
//	glPushMatrix();
//	glColor3f(1, 0, 0);
//	glTranslatef(-1.65, -0.75, -0.65);
//	glScalef(0.1, 0.1, 0.1);
//	glutSolidSphere(1.5, 20, 10);
//	glPopMatrix();
//	
//	// Make Wheel 2
//	glPushMatrix();
//	glColor3f(1, 0, 0);
//	glTranslatef(-1.65, 0.75, 0.65);
//	glScalef(0.1, 0.1, 0.1);
//	glutSolidSphere(1.5, 20, 10);
//	glPopMatrix();
//	
//	// Make Wheel 3
//	glPushMatrix();
//	glColor3f(1, 0, 0);
//	glTranslatef(-1.65, -0.75, 0.65);
//	glScalef(0.1, 0.1, 0.1);
//	glutSolidSphere(1.5, 20, 10);
//	glPopMatrix();
//	
//	// Make Wheel 3
//	glPushMatrix();
//	glColor3f(1, 0, 0);
//	glTranslatef(-1.65, 0.75, -0.65);
//	glScalef(0.1, 0.1, 0.1);
//	glutSolidSphere(1.5, 20, 10);
//	glPopMatrix();
//	
//	glPushMatrix();
//	// Roatate the sperhical base
//	glRotatef((GLfloat)arm_angles[BASE_X], 1, 0, 0); // Rotation for X
//	
//	// Make base
//	glPushMatrix();
//	glColor3f(0.5, 0.5, 0.5); // Make it grey
//	glutSolidSphere(1, 20, 10); // Draw the base as a solid sphere
//	glColor3f(1,0,0); // Make it red
//	glutWireSphere(1, 20, 10); // Draw the base as a spherical wire frame
//	glPopMatrix();
//	
//	glScalef(0.75, 0.75, 0.75); // scale to make arm smaller
//	
//	glTranslatef(1.0, 0, 0); // Translate
//	glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0., 0., 1.); // Rotation for Z
//	glTranslatef(1.5, 0, 0); // Translate
//	glPushMatrix();
//	glScalef(3.0, 0.3, 0.3);
//	glColor3f(0.25, 0.25, 0.25); // Make it a dark shade grey
//	glutSolidCube(1); // Draw the shoulder link
//	glPopMatrix();
//	
//	glTranslatef(1.5, 0, 0); // Translate
//	glRotatef((GLfloat)arm_angles[ELBOW_Z], 0., 0., 1.); //rotation on Z
//	glTranslatef(1.0, 0, 0); // Translate
//	glPushMatrix();
//	glScalef(2.0, 0.3, 0.3);
//	glColor3f(0.5, 0.5, 0.5); // Make it a normal shade of grey
//	glutSolidCube(1); // Draw the elbow link
//					  // glutWireCube(1); // Wire Cube to make it more fancy
//	glPopMatrix();
//	
//	glTranslatef(1.0, 0, 0); // Translate
//	glRotatef((GLfloat)arm_angles[WRIST_X], 1, 0, 0); //rotation on X
//	
//	glTranslatef(0.5, 0, 0); // Translate
//	glPushMatrix();
//	glScalef(1.0, 0.3, 0.3);
//	glColor3f(0.75, 0.75, 0.75); // Make it ligher shade of grey
//	glutSolidCube(1); // Draw the end effector (wrist)
//	glPopMatrix();
//	
//	glTranslatef(0.5, 0, 0);
//	glRotatef((GLfloat) arm_angles[PENCIL_Z], 0., 0., 1.); //bend on Z
//	glRotatef((GLfloat) arm_angles[PENCIL_X], 1., 0., 0.); //rotation on X
//	
//	glPushMatrix();
//	glScalef(1.2, 0.2, 0.1);
//	glColor3f(1, 0, 0);
//	glutSolidCube(1);
//	glPopMatrix();
//	glPopMatrix();
//	glPopMatrix();
//	
//	glRotated(-90, 0,0,1 );
//	glTranslatef(-cx, 0, -cz);
//	
//}
//
//// Special keys to move the robot around manually
//void specialKeys(int key, int x, int y)
//{
//	// Up arrow key is pressed
//	if (key == GLUT_KEY_UP)
//	{
//		if (cz > 14)
//		{
//			cz--;
//		}
//		if (cz < 14)
//		{
//			cz++;
//		}
//		else
//		{
//			cz = 14;
//		}
//	}
//	
//	// Down arrow key is pressed
//	if (key == GLUT_KEY_DOWN)
//	{
//		if (cz > 0)
//		{
//			cz--;
//		}
//		if (cz < 0)
//		{
//			cz++;
//		}
//	}
//	
//	// Left arrow key is pressed
//	if (key == GLUT_KEY_LEFT)
//	{
//		if (cx > 0)
//		{
//			cx--;
//		}
//		if (cx < 0)
//		{
//			cx++;
//		}
//		
//	}
//	
//	// Right arrow key is pressed
//	if (key == GLUT_KEY_RIGHT)
//	{
//		if (cx > 14)
//		{
//			cx--;
//		}
//		if (cx < 14)
//		{
//			cx++;
//		}
//		else
//		{
//			cx = 14;
//		}
//	}
//	
//	glutPostRedisplay();
//}
//
//
//// Function for keyboard entry to manipulate the haptic device
//void keyboard(unsigned char key, int x, int y)
//{
//	int delta = 5;
//	
//	switch (key)
//	{
//			// Exit case by hitting 'ESC' key
//		case 27: //ESC in ASCII
//			exit(1);
//			break;
//			//			// The Base is rotated using ASWD
//			//			// Move the BASE_X joint in + & - diections
//			//		case 'a':
//			//			change_angle(BASE_X, delta, -180, 0);
//			//			break;
//			//		case 'd':
//			//			change_angle(BASE_X, -delta, -180, 0);
//			//			break;
//			//			// Move the SHOULDER_Z joint in + & - diections
//			//		case 'n':
//			//			change_angle(SHOULDER_Z, delta, 0, 135);
//			//			break;
//			//		case 'm':
//			//			change_angle(SHOULDER_Z, -delta, 0, 135);
//			//			break;
//			//
//			//			// Move the ELBOW_Z joint in + & - diections
//			//		case 'j':
//			//			change_angle(ELBOW_Z, delta, 0, 135);
//			//			break;
//			//		case 'k':
//			//			change_angle(ELBOW_Z, -delta, 0, 135);
//			//			break;
//			//
//			//			// Move the WRIST_X joint in + & - diections
//			//		case 'p':
//			//			change_angle(WRIST_X, delta, -45, 45);
//			//			break;
//			//		case 'l':
//			//			change_angle(WRIST_X, -delta, -45, 45);
//			//			break;
//			//
//			//			// Move the WRIST_X joint in + & - diections
//			//		case 'i':
//			//			change_angle(WRIST_X, delta, -15, 90);
//			//			break;
//			//		case 'o':
//			//			change_angle(WRIST_X, -delta, -15, 90);
//			//			break;
//	}
//	glutPostRedisplay();
//}
//
//
//
//// Now the fun part, putting everything together
//void drawHouse()
//{
//	// Draw a 15 x 15 x 15 room
//	drawWall(0, 0, 15, 15);
//	//viaPoint(viax, viaz);
//	
//	// haptic();
//	// Put the furniture in the room
//	
//	drawFloor();
//	makeRug(); // Rug function needs to be called first so that the furniture can be places on top of it
//	makeTable();
//	makeShelf();
//	makeChair();
//	makeTV();
//	makeRobot();
//	
//	return;
//}
//
//
//// Main Display Function
//void display()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	// Display the masterpiece on the screen
//	drawHouse();
//	
//	
//	drawHumanoid(0, 0, 14); //Standing left of Char1
//	drawHumanoid(0, 0, 10); // Standing between the chairs
//	drawHumanoid(9, 0, 1); // Standing between TV and bookshelf
//	
//	// Two people standing having a conversation
//	// drawHumanoid(10, 9, 10);
//	// drawHumanoid(9, 5, 12);
//	
//	// Uncomment below to have a humanoid party of nine
//	/*
//	 drawHumanoid(0, 0, 0);
//	 drawHumanoid(10, 8, 8);
//	 drawHumanoid(13, 14, 6);
//	 drawHumanoid(8, 0, 14);
//	 */
//	
//	glFlush();
//	
//	return;
//}
//
//
//void init()
//{
//	
//	// Set the current clear color to black and the current drawing color to white.
//	glClearColor(0.7, 0.7, 0.7, 1.0);
//	glColor3f(1.0, 1.0, 1.0);
//	
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity(); // Reset View
//	gluPerspective(90.0, 16 / 9, 1, 100);
//	
//	// Position camera at (x,y,z) looking at (0, 0, 0) with the vector
//	// <0, 1, 0> pointing upward.
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	
//	// <eye point> <reference point> <up vector>
//	// (eye X, eye Y, eye Z, center X, center Y, center, Z, up X, up Y, up Z)
//	gluLookAt(20, 10, 20, 0, 0, 0, 0, 1, 0);
//	return;
//	
//}
//
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
////	glutInitWindowPosition(80, 80);
//	glutInitWindowSize(800, 600); // Size the window
//	glutCreateWindow("ROBOTICS PROJECT");
//	
//	mainw = glutGetWindow();
//	
//	glutDisplayFunc(display);
//	glutSpecialFunc(specialKeys);
//	glutKeyboardFunc(keyboard);
//	
//	glutIdleFunc(display);
//	
//	init();
//	setupGlui();
//	glutMainLoop();
//	
//	return 0;
//}
