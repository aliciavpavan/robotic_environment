//
//  objects.h
//  openGL
//

#ifndef objects_h
#define objects_h

#ifdef _WIN32
#include <GL/glut.h>
#include "glui.h"
#elif __APPLE__
#include <GLUT/glut.h>
#include <GLUI/glui.h>
#elif __LINUX__
#include <GL/glut.h>
#endif

extern double arm_angles[6];
enum { BASE_X, SHOULDER_Z, ELBOW_Z, WRIST_X, PENCIL_Z, PENCIL_X };
extern GLfloat cameraDistance;
extern GLfloat cameraAngle;
extern int obs_check[17][17];
extern bool turn;
extern int cx,cy,cz;
extern int rotate, wheelturn;

void printobs_check(void);

void drawSquarePrism(float, float, float, float);
void drawRectPrism(float, float, float, float, float, float);
void drawWall(float, float, float, float);
void drawFloor(void);
void makeTable(void);
void makeRug(void);
void makeShelf(void);
void makeChair(void);
void makeTV(void);
void drawHumanoid(int, int, int);
void LineStipple(float, float, float, float, float, float);
void makeArm(void);
void makePlatform(void);
void makeWheel4(void);
void makeWheel3(void);
void makeWheel2(void);
void makeWheel1(void);
void makeWheel(void);
void makeCoordinateFrame(void);
void drawGrid(double);
void drawtext(char *string,float x,float y);
void drawRamp(void);

#endif /* objects_h */
