//
//  tipping.hpp
//  openGL
//
//  Created by Alicia Pavan on 2018-04-15.
//  Copyright © 2018 alicia. All rights reserved.
//

#ifndef tipping_h
#define tipping_h

#ifdef _WIN32
#include <GL/glut.h>
#include "glui.h"
#elif __APPLE__
#include <GLUT/glut.h>
#include <GLUI/glui.h>
#elif __LINUX__
#include <GL/glut.h>
#endif

void TippingCase2();
void Torque();
void TippingCase1();
double obstacleTest();

#endif /* tipping_hpp */
