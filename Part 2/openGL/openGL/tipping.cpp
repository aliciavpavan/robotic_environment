/*******************************************|
 TIPPING FUNCTIONS						|
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
#include <stdio.h>

#include "tipping.h"
#include "objects.h"

float dfo;//distance from original

extern double theta, mass, obsheight;
extern bool ramptipping, weighttipping;
double m1 = 30; //mass of robot
double pi = 3.14159;
double t1 = 0, t2 = 0;

double obstacleTest()
{
	theta = atan(obsheight/20)*180/pi; // 20 is the length of platform (two squares)
	return theta;	
}

void TippingCase1()// ramp case test
{
	// if smaller enough, move over it
	obstacleTest();
	std::cout<<"THETA: "<<theta<<std::endl;
	if( theta >= 30 || cx > 15 || cz > 15)
	{
		std::cout << "Robot is tipping over!" << std::endl;
		ramptipping = true;
	}
	else
	{
		std::cout << "Robot can move over the obstacle!" << std::endl;
		ramptipping = false;
	}
}

void TippingCase2()//dynamics test
{
	Torque();
	if (t2 >= t1)
	{
		std::cout << "Exceeds the maximum load!" << std::endl;
		weighttipping = true;
	}
	else
	{
		std::cout << "Static Balance!" << std::endl;
		weighttipping = false;
	}
}

void Torque() ///// to see the dynamic balance
{
	double theta2=135; /////angle between end effector and body
	double l1 = 1; ////height of robot
	double l2 = 0.5; ////// tool arm with end effector
	
	double s2 = sin(theta2 * pi / 180);
	double c2 = cos(theta2 * pi / 180);
	t1 = (l1 * s2 * m1*9.8) + mass*9.8 * (l2 + l1 * c2);
	t2 = l2 * mass*9.8;
}

