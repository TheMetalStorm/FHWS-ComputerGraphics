// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

//#define TEAPOT
#define MODEL

#define PERSPECTIVE
//#define ORTHO

//#define ROBOT


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

float screenWidth, screenHeight = 1.0f;
void renderCube(float r, float g, float b) {

    float frontColor[3] = {r, g, b};
    float backColor[3] = {r * 0.8f, g * 0.8f, b * 0.8f};
    float topColor[3] = {r * 0.6f, g * 0.6f, b * 0.6f};
    float bottomColor[3] = {r * 0.4f, g * 0.4f, b * 0.4f};
    float rightColor[3] = {r * 0.2f, g * 0.2f, b * 0.2f};
    float leftColor[3] = {r * 0.1f, g * 0.1f, b * 0.1f};

    // Draw the cube
    glBegin(GL_QUADS);
    // Front face
    glColor3fv(frontColor);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    // Back face
    glColor3fv(backColor);

    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    // Top face
    glColor3fv(topColor);

    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    // Bottom face
    glColor3fv(bottomColor);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    // Right face
    glColor3fv(rightColor);

    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    // Left face
    glColor3fv(leftColor);

    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();
    glFlush();
}

void drawDumbRobot(){
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 6, 15, 0, 0, 0, 0, 1, 0);

    //Base
    glTranslatef(xTranslation, yTranslation, 0);
    glScalef(4,1,2);
    renderCube(1,0,0);

    glPushMatrix();

    //UpperArm
    glTranslatef(-.6f,2.5,0);
    glScalef(.1,1.5f,.3);
    renderCube(0,1,0);

    //LowerArm
    glTranslatef(0,1.3,0);
    glRotatef(angleX, 0,1,0);
    glScalef(4,.3,1);
    glTranslatef(-.75f,0,0);
    renderCube(0,1,1);

    //Hand
    glTranslatef(-1.25f,0,0);
    glRotatef(angleY, 1,0,0);
    glScalef(.25,2,1);
    renderCube(1,1,0);

    glPopMatrix();

    //UpperArm
    glTranslatef(.6f,2.5,0);
    glScalef(.1,1.5f,.3);
    renderCube(0,1,0);

    //LowerArm
    glTranslatef(0,1.3,0);
    glRotatef(angleX, 0,1,0);
    glScalef(4,.3,1);
    glTranslatef(.75f,0,0);
    renderCube(0,1,1);

    //Hand
    glTranslatef(1.25f,0,0);
    glRotatef(angleY, 1,0,0);
    glScalef(.25,2,1);
    renderCube(1,1,0);
}

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
#if defined(MODEL) || defined(TEAPOT)
    auto viewingMat =  glm::lookAt(glm::vec3(0, 0, -4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

#if defined(ORTHO)
    auto projectionMat =  glm::ortho(0.f, screenWidth/screenHeight, 0.0f, 1.0f, -1.f, 50.0f);
#elif defined(PERSPECTIVE)
    auto projectionMat = glm::perspective(30.0f, screenWidth/screenHeight, .1f, 300.0f);
#endif
    tblock tblock;
    tblock.transform = glm::translate(glm::vec3(xTranslation, yTranslation, -2));
    tblock.transform *=glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    tblock.transform *=glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(.0f, 1.0f, 0.0f));
    tblock.transform *= glm::scale(glm::vec3(scale,scale,scale));
#endif

#if defined(TEAPOT)
    #if defined(PERSPECTIVE) || defined(ORTHO)
    tblock.look = viewingMat;
    #endif
    tblock.proj = projectionMat;
    GLuint blockIndex = glGetUniformBlockIndex(prg, "TBlock");
    GLuint uBuf;
    glGenBuffers(1, &uBuf);
    glBindBuffer(GL_UNIFORM_BUFFER, uBuf);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(tblock), &tblock, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uBuf);
    glutSolidTeapot(.5f);
//    glutWireTeapot(.5f);
#endif
#if defined(ROBOT)
    drawDumbRobot();
#endif

#if defined(MODEL)
    models[currentModelIndex]->tblock.transform = tblock.transform;
    models[currentModelIndex]->transform(prg, viewingMat, projectionMat);
    models[currentModelIndex]->render(false);
#endif

    glutSwapBuffers();
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

void ChangeSizePerspective(GLsizei w, GLsizei h){

// Set Viewport to window dimensions
        glViewport(0, 0, w, h);
        GLfloat fAspect = (GLfloat)w/(GLfloat)h;
// Reset coordinate system
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
// Produce the perspective projection
        gluPerspective(60.0f, fAspect, 1.0, 400.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

}

void ChangeSize(GLsizei w, GLsizei h)
{
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glViewport(0, 0, w, h);
//    gluPerspective(40, (float) w / (float)h, 1, 100);
//    projectionMat = glm::perspective(90.0f,  (GLfloat)w / (GLfloat)h, 1.0f, 300.0f);
//    GLfloat projectionArray[16];
//    memcpy(projectionArray, glm::value_ptr(projectionMat), sizeof(float) * 16);
//    glMultMatrixf(projectionArray);
//        if(w > h)
        glViewport(0,0, w, h);
        screenWidth = (float)w;
        screenHeight = (float)h;
//    projectionMat = glm::perspective(90.0f, (float)w/(float)h, 1.0f, 300.0f);

//    else
//        glViewport(0,0, w, w);
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

std::string readShaderFile(const char *filePath) {
    // no feedback is provided for stream errors / exceptions.

    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void setupShaders(){
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    prg = glCreateProgram();

    const char* vShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\VisualizeNormals.vert)").c_str();
    const char* fShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\VisualizeNormals.frag)").c_str();
    glShaderSource(vs, 1, &vShader, nullptr);
    glCompileShader(vs);
    glShaderSource(fs, 1, &fShader, nullptr);
    glCompileShader(fs);


    glAttachShader(prg, vs);
    glAttachShader(prg, fs);

    glLinkProgram(prg);
    glUseProgram(prg);
}
// Setup the rendering state
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    GLenum err = glewInit();
    #if defined(MODEL) || defined(TEAPOT)
        setupShaders();
    #endif
    #if defined(MODEL)
        setupModels();
    #endif
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 	glutCreateWindow("Simple");
//    glutTimerFunc(33, TimerFunction, 1);
    glutMouseFunc(mouseFunc);
    glutSpecialFunc(SpecialKeyboardFunc);

    glutKeyboardFunc(KeyboardFunc);

    #if defined(MODEL)|| defined(TEAPOT)
        glutReshapeFunc(ChangeSize);
    #elif defined(ROBOT)
        glutReshapeFunc(ChangeSizePerspective);
    #endif
    glutDisplayFunc(RenderScene);

	SetupRC();
	glutMainLoop();
    for (const auto &item: models){
        delete item;
    }
    return 0;
}

