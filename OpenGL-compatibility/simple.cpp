// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

void drawRect(float x, float y);
void drawMovingRectTranslate(float x, float y);
void drawMovingRectTranslateRotateScale(float x, float y);

#include <windows.h>		// Must have for Windows platform builds
#include "gl/glew.h"
#include "glut.h"			// Glut (Free-Glut on Windows)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/transform.hpp"
#include <math.h>
#include <iostream>
#include "simple.h"
#include "Mesh.h"
#include "shaders.h"

GLfloat xTranslation = 0.0f;
GLfloat yTranslation = 0.0f;
GLfloat translationFactor = .1f;
GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleChangeSpeed = 5;
GLfloat scale = 3.0f;

float scaleFactor = 0.2;

GLfloat xstep = 0.03f;
GLfloat ystep = 0.02f;

bool drawCube = true;
vector<Mesh*> models = {};
int currentModelIndex = 0;

GLuint prg;
auto viewingMat =  glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
auto projectionMat =  glm::ortho(0.f, 1.0f, 0.0f, 1.0f, -1.f, 50.0f);
///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //a
//    drawRect(0,0);
    //b
//    drawRect(x,y);
    //c)
//    drawMovingRectTranslate(x, y);
    //d
//    drawMovingRectTranslateRotateScale( x,  y);


    tblock tblock;
    tblock.transform = glm::translate(glm::vec3(xTranslation, yTranslation, -2));
    tblock.transform *=glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    tblock.transform *=glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(.0f, 1.0f, 0.0f));
    tblock.transform *= glm::scale(glm::vec3(scale,scale,scale));

    //6.1

//    tblock.look = viewingMat;
//    tblock.proj = projectionMat;
//    GLuint blockIndex = glGetUniformBlockIndex(prg, "TBlock");
//    GLuint uBuf;
//    glGenBuffers(1, &uBuf);
//    glBindBuffer(GL_UNIFORM_BUFFER, uBuf);
//    glBufferData(GL_UNIFORM_BUFFER, sizeof(tblock), &tblock, GL_DYNAMIC_DRAW);
//    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uBuf);
//    glutSolidTeapot(.5f);
//    glutWireTeapot(.5f);

//6.3
    models[currentModelIndex]->tblock.transform = tblock.transform;
    models[currentModelIndex]->transform(prg, viewingMat, projectionMat);
    models[currentModelIndex]->render(false);

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
    glRotatef(angleX, 0, 0, 1);
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
void setupModels(){
    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\cube.obj)"));
    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\sphere.obj)"));
    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\lucy\lucy.obj)"));

    for (const auto &item: models){
        item->init();
    }
}

void setupShaders(){
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
    prg = glCreateProgram();

    glShaderSource(vs, 1, &vShader, nullptr);
    glCompileShader(vs);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);

    glAttachShader(prg, vs);
    glAttachShader(prg, fragmentShaderOrange);

    glLinkProgram(prg);
    glUseProgram(prg);
}
// Setup the rendering state
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    GLenum err = glewInit();
    setupShaders();
    setupModels();
    glEnable(GL_DEPTH_TEST);

}

void TimerFunction(int value)
{
    if(xTranslation > 1 - 0.25 || xTranslation < -1+0.25)
        xstep = -xstep;
    if(yTranslation > 1 - 0.25 || yTranslation < -1 + 0.25)
        ystep = -ystep;
    xTranslation += xstep;
    yTranslation += ystep;

    float rotationFactor = 5.0f;
    angleX += rotationFactor;
//    angleX = fmod(angleX, 360);

    if (scale > 2   || scale<0) {
       scaleFactor = -scaleFactor;
    }
    scale += scaleFactor;

    glutPostRedisplay();
    glutTimerFunc(33,TimerFunction,1);
}
void SpecialKeyboardFunc(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        xTranslation-= translationFactor;
    else if(key == GLUT_KEY_RIGHT )
        xTranslation+= translationFactor;
    else if (key == GLUT_KEY_UP)
        yTranslation-= translationFactor;
    else if(key == GLUT_KEY_DOWN )
        yTranslation+= translationFactor;


    cout <<xTranslation<< endl;
    cout <<yTranslation<< endl;
    glutPostRedisplay();

}

void KeyboardFunc(unsigned char key, int x, int y) {

    if (key == 'd')
        angleY += angleChangeSpeed;
    else if(key == 'a')
        angleY -= angleChangeSpeed;
    if (key == 'w')
        angleX += angleChangeSpeed;
    else if(key == 's')
        angleX -= angleChangeSpeed;

    //space
    if (key == 32)
        currentModelIndex = ++currentModelIndex % models.size();

    glutPostRedisplay();


}

void mouseFunc(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        switch (button) {

            case 3:  //mouse wheel scrolls
                scale+=scaleFactor;
                break;
            case 4:
                if (scale > 0.2) {
                    scale-=scaleFactor;
                }
                break;
            default:
                break;
        }
    }



    glutPostRedisplay();
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
 	glutCreateWindow("Simple");
//    glutTimerFunc(33, TimerFunction, 1);
    glutMouseFunc(mouseFunc);
    glutSpecialFunc(SpecialKeyboardFunc);

    glutKeyboardFunc(KeyboardFunc);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

	SetupRC();
	glutMainLoop();
    for (const auto &item: models){
        delete item;
    }
    return 0;
}

