// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

void drawRect(float x, float y);
void drawMovingRectTranslate(float x, float y);
void drawMovingRectTranslateRotateScale(float x, float y);

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)

#include <math.h>
#include <iostream>

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat angle = 0.0f;
GLfloat scale = 0.0f;

float scaleFactor = 0.05;

GLfloat xstep = 0.03f;
GLfloat ystep = 0.02f;

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
    //a
//    drawRect(0,0);
    //b
//    drawRect(x,y);
    //c)
//    drawMovingRectTranslate(x, y);
    //d
    drawMovingRectTranslateRotateScale( x,  y);

    // Flush drawing commands
    glFlush();
}

void drawRect(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(x-0.25f,y-0.25f,x+0.25f,y+0.25f);
}


void drawMovingRectTranslate(float x, float y){
    glColor3f(1.0f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x,y,0);
    glRectf(-0.25f, -0.25f, 0.25f, 0.25f);
}

void drawMovingRectTranslateRotateScale(float x, float y){
    glColor3f(1.0f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x,y,0);
    glRotatef(angle, 0, 0, 1);
    glScalef(scale, scale, 0);
    glRectf(-0.25f, -0.25f, 0.25f, 0.25f);
}

void ChangeSize(GLsizei width, GLsizei height)
{
    if(width > height)
        glViewport(0,0, height, height);
    else
        glViewport(0,0, width, width);
}
///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void TimerFunction(int value)
{
    if(x > 1 - 0.25 || x < -1+0.25)
        xstep = -xstep;
    if(y > 1 - 0.25 || y < -1+0.25)
        ystep = -ystep;
    x += xstep;
    y += ystep;

    float rotationFactor = 0.5f;
    angle += rotationFactor;
    angle = fmod(angle, 360);

    if (scale > 2   || scale<0) {
       scaleFactor = -scaleFactor;
    }
        scale += scaleFactor;

    glutPostRedisplay();
    glutTimerFunc(33,TimerFunction,1);
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
 	glutCreateWindow("Simple");
    glutTimerFunc(33, TimerFunction, 1);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

	SetupRC();

	glutMainLoop();
    
    return 0;
}

