/*******************************************|
 OBJECTS						|
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

// Function for drawing a square prism
void drawSquarePrism(float x, float z, float height, float width)
{
	// This fuction takes the floor as a x.y coordinates on the floor
	
	glBegin(GL_QUADS);
	
	//Left side
	glVertex3f(x, 0, z + width); // Top Right
	glVertex3f(x, 0, z); // Top Left
	glVertex3f(x, height, z); // Bottom Left
	glVertex3f(x, height, z + width); // Bottom Right
	
	//Right Side
	glVertex3f(x + width, 0, z + width);
	glVertex3f(x + width, 0, z);
	glVertex3f(x + width, height, z);
	glVertex3f(x + width, height, z + width);
	
	//Front Side
	glVertex3f(x, 0, z + width);
	glVertex3f(x + width, 0, z + width);
	glVertex3f(x + width, height, z + width);
	glVertex3f(x, height, z + width);
	
	//Top Face
	glVertex3f(x, height, z);
	glVertex3f(x, height, z + width);
	glVertex3f(x + width, height, z + width);
	glVertex3f(x + width, height, z);
	
	//Back Side
	glVertex3f(x + width, 0, z);
	glVertex3f(x, 0, z);
	glVertex3f(x, height, z);
	glVertex3f(x + width, height, z);
	
	//Bottom Face
	glVertex3f(x, 0, z);
	glVertex3f(x, 0, z + width);
	glVertex3f(x + width, 0, z + width);
	glVertex3f(x + width, 0, z);
	
	glEnd();
	return;
}


// Function for drawing a regular (non-square) prism
void drawRectPrism(float x, float z, float y_offset, float height, float widthx, float widthz)
{
	// Unlike the squarePrism function, this one takes the z axis into account
	
	glBegin(GL_QUADS);
	
	//Left side
	glVertex3f(x, y_offset, z + widthz); // Top Right
	glVertex3f(x, y_offset, z); // Top Left
	glVertex3f(x, y_offset + height, z); // Bottom Left
	glVertex3f(x, y_offset + height, z + widthz); // Bottom Right
	
	//Right Side
	glVertex3f(x + widthx, y_offset, z + widthz);
	glVertex3f(x + widthx, y_offset, z);
	glVertex3f(x + widthx, y_offset + height, z);
	glVertex3f(x + widthx, y_offset + height, z + widthz);
	
	//Front Side
	glVertex3f(x, y_offset, z + widthz);
	glVertex3f(x + widthx, y_offset, z + widthz);
	glVertex3f(x + widthx, y_offset + height, z + widthz);
	glVertex3f(x, y_offset + height, z + widthz);
	
	//Top Face
	glVertex3f(x, y_offset + height, z);
	glVertex3f(x, y_offset + height, z + widthz);
	glVertex3f(x + widthx, y_offset + height, z + widthz);
	glVertex3f(x + widthx, y_offset + height, z);
	
	//Back Side
	glVertex3f(x + widthx, y_offset, z);
	glVertex3f(x, y_offset, z);
	glVertex3f(x, y_offset + height, z);
	glVertex3f(x + widthx, y_offset + height, z);
	
	//Bottom Face
	glVertex3f(x, y_offset, z);
	glVertex3f(x, y_offset, z + widthz);
	glVertex3f(x + widthx, y_offset, z + widthz);
	glVertex3f(x + widthx, y_offset, z);
	
	glEnd();
	return;
}


// Drawing dashed lines to connect objects in the room
void LineStipple(float x1, float y1, float z1, float x2, float y2, float z2)
{
	glColor3f(0.0f, 0.0f, 0.0f); // Set color to black
	glLineWidth(1.0);
	glLineStipple(4, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);	// Begining of the line
	glVertex3f(x2, y2, z2);	// End of the line
	
	glDisable(GL_LINE_STIPPLE);
	
	glEnd();
}

// Draw the walls of the room
void drawWall(float x1, float z1, float x2, float z2) {
	
	// Make the walls pink
	glColor3f(0.7, 0.1, 0.6);
	
	glRectf(x1, z1, x2, z2);
	glRotatef(-90, 0, 1, 0);
	glRectf(x1, z1, x2, z2);
	glRotatef(90, 0, 1, 0);
	//make walls as obstacles in grid
	for(int x=0;x<17;x++){
		obs_check[0][x]=7;
		obs_check[16][x]=7;
	}
	for(int z=0;z<17;z++){
		obs_check[z][0]=7;
		obs_check[z][16]=7;
	}
	
	return;
}

void drawFloor(void)
{
	// Make it white
	glColor3f(0.95, 0.5, 0.5);
	
	// Draw the rug on the floor
	drawRectPrism(0, 0, 0, -0.1, 15, 15);
	
	/// Note: The rug must be drawn first to allow for furniture to be placed on top of it
}

void drawGrid(double size){
	glPushMatrix();
	glTranslated(size, 0, size);
	glBegin(GL_LINES);
	glColor3f(0.7, 0.3, 0.3);
	for(double i=-size;i<=size+0.5;i++)
	{
		
		glVertex3f((float)i,0,(float)-size);
		glVertex3f((float)i,0,(float)size);
		
		glVertex3f((float)-size,0,(float)i);
		glVertex3f((float)size,0,(float)i);
		
	}
	glEnd();
	glPopMatrix();
}

// Fucntion to make a nice simple table
void makeTable(void)
{
	// Table will have 4 legs and a table surface
	glPushMatrix();
	glTranslated(3, 0, 2); //move it
						   // Make the table blue
	glColor3f(0, 0, 1);
	
	// Table Position 1
	// Draw table legs as square prisms Legs
	drawSquarePrism(4, 4, 1.5, 0.1);
	drawSquarePrism(4, 5.9, 1.5, 0.1);
	drawSquarePrism(6.9, 4, 1.5, 0.1);
	drawSquarePrism(6.9, 5.9, 1.5, 0.1);
	
	//Table Surface using the RectPrism
	drawRectPrism(4, 4, 1.25, 0.25, 3, 2);
	
	//CREATING OBSTACLE IN ARRAY
	
	obs_check[7][8]=1;
	obs_check[7][9]=1;
	obs_check[7][10]=1;
	
	obs_check[8][8]=1;
	obs_check[8][9]=1;
	obs_check[8][10]=1;
	
	glPopMatrix();
	return;
}


// Function to draw a rug on the floor
void makeRug(void)
{
	
	// Draw the rug on the floor
	drawRectPrism(5, 3, 0, -0.1, 6, 8);
	
	/// Note: The rug must be drawn first to allow for furniture to be placed on top of it
}

// Function to make a bookshelf
void makeShelf(void)
{
	
	// Simple bookshelf here
	
	// Make it neon green, because all good shelves should be neon colored
	glColor3f(0, 1, 0);
	
	drawSquarePrism(11, 0, 4, 2);	// This shelf will go after the TV in the far end of the room
	
	obs_check[1][12]=1;
	obs_check[1][13]=1;
	obs_check[2][12]=1;
	obs_check[2][13]=1;
	return;
}

// Function to make a chair
void makeChair(void)
{
	// Chair will have 4 legs, a seat, and a back rest
	
	// Make it yellow, beacuse yellow colored chairs are the best chairs
	glColor3f(1, 1, 0);
	
	// CHAIR ONE \\\
	
	// Chair legs
	drawSquarePrism(2, 12, 1.6, 0.1);
	drawSquarePrism(1, 12, 1.6, 0.1);
	drawSquarePrism(2, 13, 1.6, 0.1);
	drawSquarePrism(1, 13, 1.6, 0.1);
	
	// Seat for the chair
	drawRectPrism(1, 12, 1.7, 0.25, 1.2, 1.1);
	
	// Back rest for the chair
	drawRectPrism(1, 12, 1.7, 1.5, 0.2, 1);
	//	obs_check[6][1]=1;
	//	obs_check[7][1]=1;
	obs_check[9][2]=1;
	/// CHAIR TWO  \\\
	
	// Chair legs
	drawSquarePrism(2, 8, 1.6, 0.1);
	drawSquarePrism(1, 8, 1.6, 0.1);
	drawSquarePrism(2, 9, 1.6, 0.1);
	drawSquarePrism(1, 9, 1.6, 0.1);
	
	// Seat for the chair
	drawRectPrism(1, 8, 1.7, 0.2, 1.2, 1.1);
	
	// Back rest for the chair
	drawRectPrism(1, 8, 1.7, 1.5, 0.2, 1);
	//	obs_check[10][1]=1;
	//	obs_check[11][1]=1;
	obs_check[13][2]=1;
	
}


// Function to draw a TV and the cable box
void makeTV(void)
{
	// All households have a TV and this one should have one too
	
	// Make TV and wire black
	glColor3f(0, 0, 0);
	
	// Draw a wall mounted flat screen TV
	drawRectPrism(5, 0, 5, 3, 4, 0.01);
	
	// Draw the power cable bcause TV's need electricity
	drawRectPrism(7, 0, 2, 5, 0.1, 0.01);
	
	// Change color of the cable box stand/table
	glColor3f(0, 0.25, 0);
	
	// This is the TV stand/shelf where you put the cable box and some DVDs
	drawRectPrism(6, 0, 0, 2, 2, 1);
	
	obs_check[1][7]=1;
	obs_check[1][8]=1;
	
}


// Function to draw a very simple "humanoid"
void drawHumanoid(int x, int y, int z)
{
	// Draw a beautiful humanoid
	// More detail can be added later on if we were to expand more on this project
	
	const float height = 3;
	const float width = 0.5;
	const float headradius = 0.5;
	const float armwidth = 0.25;
	
	// The body will just be a square prism
	glColor3f(0.25, 0.25, 0.25); // colored torso/body
	drawSquarePrism(x, z, height, width);
	
	// Use sphere for head
	glTranslatef(x + width / 2, height + headradius, z + width / 2); // Move to draw head
	glColor3f(1, 0.8, 0.5); // some tan colored skintone
	glutSolidSphere(headradius, 10, 10);
	glTranslatef(-(x + width / 2), -(height + headradius), -(z + width / 2)); // Move back to origin
	
	// Draw some arms for the humanoid
	drawRectPrism(x + width / 2, z - armwidth, height*.6, 1, armwidth, armwidth);
	drawRectPrism(x + width / 2, z + width, height*.6, 1, armwidth, armwidth);
	
}

void makePlatform(void){
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.7);
	glScalef(0.25, 1, 1);
	glutSolidCube(1.5);
	glPopMatrix();
}

void makeWheel1()
{
	glPushMatrix();
	glTranslatef(-0.15, -0.75, -0.65);
	glScalef(0.1, 0.1, 0.1);
	makeWheel();
	glPopMatrix();
}

void makeWheel2()
{
	glPushMatrix();
	glTranslatef(-0.15, 0.75, 0.65);
	glScalef(0.1, 0.1, 0.1);
	makeWheel();
	glPopMatrix();
}

void makeWheel3()
{
	glPushMatrix();
	glTranslatef(-0.15, -0.75, 0.65);
	glScalef(0.1, 0.1, 0.1);
	makeWheel();
	glPopMatrix();
}

void makeWheel4()
{
	glPushMatrix();
	glTranslatef(-0.15, 0.75, -0.65);
	glScaled(0.1, 0.1, 0.1);
	makeWheel();
	glPopMatrix();
	
}

void makeWheel(void){
	glColor3f(0, 0, 0);
	
	glutSolidTorus(1, 3, 20, 10);
	//Movement rotation
	glRotated(wheelturn, 0, 0, 1);
	
	glRotatef(90, 0, 1, 0);
	glColor3f(1, 1, 1);
	glutSolidCone(0.25, 3, 20, 10);
	glColor3f(0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glutSolidCone(0.25, 3, 20, 10);
	
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 0, 0);
	glutSolidCone(0.25, 3, 20, 10);
	glColor3f(0, 1, 0);
	glRotatef(180, 1, 0, 0);
	glutSolidCone(0.25, 3, 20, 10);
}

void makeArm(void)
{
	glPushMatrix();
	// Roatate the sperhical base
	glRotatef((GLfloat)arm_angles[BASE_X], 1, 0, 0); // Rotation for X
	
	// Make base
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5); // Make it grey
	glutSolidSphere(1, 20, 10); // Draw the base as a solid sphere
	glColor3f(1,0,0); // Make it red
	glutWireSphere(1, 20, 10); // Draw the base as a spherical wire frame
	glPopMatrix();
	
	glScalef(0.75, 0.75, 0.75); // scale to make arm smaller
	
	glTranslatef(1.0, 0, 0); // Translate
	glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0., 0., 1.); // Rotation for Z
	glTranslatef(1.5, 0, 0); // Translate
	glPushMatrix();
	glScalef(3.0, 0.3, 0.3);
	glColor3f(0.25, 0.25, 0.25); // Make it a dark shade grey
	glutSolidCube(1); // Draw the shoulder link
	glPopMatrix();
	
	glTranslatef(1.5, 0, 0); // Translate
	glRotatef((GLfloat)arm_angles[ELBOW_Z], 0., 0., 1.); //rotation on Z
	glTranslatef(1.0, 0, 0); // Translate
	glPushMatrix();
	glScalef(2.0, 0.3, 0.3);
	glColor3f(0.5, 0.5, 0.5); // Make it a normal shade of grey
	glutSolidCube(1); // Draw the elbow link
					  // glutWireCube(1); // Wire Cube to make it more fancy
	glPopMatrix();
	
	glTranslatef(1.0, 0, 0); // Translate
	glRotatef((GLfloat)arm_angles[WRIST_X], 1, 0, 0); //rotation on X
	
	glTranslatef(0.5, 0, 0); // Translate
	glPushMatrix();
	glScalef(1.0, 0.3, 0.3);
	glColor3f(0.75, 0.75, 0.75); // Make it ligher shade of grey
	glutSolidCube(1); // Draw the end effector (wrist)
	glPopMatrix();
	
	glTranslatef(0.5, 0, 0);
	glRotatef((GLfloat) arm_angles[PENCIL_Z], 0., 0., 1.); //bend on Z
	glRotatef((GLfloat) arm_angles[PENCIL_X], 1., 0., 0.); //rotation on X
	
	glPushMatrix();
	glScalef(1.2, 0.2, 0.1);
	glColor3f(1, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
}

void makeCoordinateFrame(void)
{
	// Planar movement of the platform is along the Red/Blue Axis
	
	// Blue Axis
	glPushMatrix();
	
	glColor3f(0, 0, 1);
	glutSolidCone(0.1, 0.75, 5, 10);
	
	// Red Axis
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 0, 0);
	glutSolidCone(0.1, 0.75, 5, 10);
	
	// Yellow Axis
	glRotatef(90, 0, 1, 0);
	glColor3f(1, 1, 0);
	glutSolidCone(0.1, 0.75, 5, 10);
	
	glPopMatrix();
	
	glScalef(0.5, 0.5, 0.5);
	glRotated(-90, 0, 0, 1);
}
void drawRamp(void)
{
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(12, 0, 12);
	glRotatef(180, 0, 1, 0);

	glScalef(0.35, 0.40, 2.5);
	glutSolidCube(1);
	
	// double size = 0;
	double size = 0.1;
	int count = 0;
	while (size < 1)
	{
		// Alternate color show the curvature of the ramp
		if (count % 2 == 0)
		{
			glColor3f(0, 0, 0); // Black
		}
		else
		{
			glColor3f(0, 0, 1); // Blue
		}
		
		glTranslated(size, -size, 0.0);
		glScalef(1, 0.8, 1);
		glutSolidCube(1);
		
		size = size + 0.1;
		count++;
	}
	
	glPopMatrix();
}


void printobs_check(){
	for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < 17; ++j)
		{
			std::cout << obs_check[i][j] <<" ";
		}
		std::cout << std::endl;
	}
}

void drawtext(char *string,float x,float y)
{
	char *c;
	glRasterPos3f(x, 0, y);
	
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}

