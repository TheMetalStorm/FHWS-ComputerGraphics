// Simple.cpp
// The Simplest OpenGL program with GLUT

#define FREEGLUT_STATIC

//#define SHADER_VIZ_NORMALS
//#define SHADER_TOON
//#define SHADER_GOURAUD
#define SHADER_PHONG

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
#include <stdio.h>
#include "../texture/texture/moonmap.h"
#include "../texture/texture/orange.h"
#include "../texture/texture/wmap.h"


GLfloat xTranslation = 0.0f;
GLfloat yTranslation = 0.0f;
GLfloat translationFactor = .1f;
GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleChangeSpeed = 5;
GLfloat scale = 3.0f;

float scaleFactor = 0.1;

GLfloat xstep = 0.03f;
GLfloat ystep = 0.02f;

vector<Mesh*> models = {};
int currentModelIndex = 0;

GLuint prg;

float screenWidth, screenHeight = 1.0f;
glm::vec4 ambient = { 0.0, 0.3, 0.2, 1.0 };
glm::vec4 diffuse = { 0.8, 1.0, 0.9, 1.0 };
glm::vec4 specular = { 1.0, 1.0, 1.0, 1.0 };
glm::vec4 lightDirection = glm::normalize(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));


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
    auto viewingMat =  glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

#if defined(ORTHO)
    auto projectionMat =  glm::ortho(0.f, screenWidth/screenHeight, 0.0f, 1.0f, -1.f, 50.0f);
#elif defined(PERSPECTIVE)
    auto projectionMat = glm::perspective(30.0f, screenWidth/screenHeight, .1f, 300.0f);
#endif
    tblock tblock;
    tblock.transform = glm::translate(glm::vec3(xTranslation, yTranslation, 0));
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
    models[currentModelIndex]->render(prg, false);
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

string generateCube(float size) {

    std::string sizeString = std::to_string(size/2);
    std::string negativeSizeString = std::to_string(-size/2);

    return "# cube.obj\n"
           "#\n"
           "\n"
           "o cube\n"
           "\n"
           "v "+ negativeSizeString +" "+ negativeSizeString +" "+ sizeString +"\n"
                                                 "v "+ sizeString +" "+ negativeSizeString +" "+ sizeString + "\n"
           "v "+ negativeSizeString +" "+ sizeString +" "+ sizeString +"\n"
           "v "+ sizeString +" "+ sizeString +" "+ sizeString +"\n"
           "v "+ negativeSizeString +" "+ sizeString +" "+ negativeSizeString +"\n"
           "v "+ sizeString +" "+ sizeString +" "+ negativeSizeString +"\n"
           "v "+ negativeSizeString +" "+ negativeSizeString +" "+ negativeSizeString +"\n"
           "v "+ sizeString +" "+ negativeSizeString +" "+ negativeSizeString +"\n"
           "\n"
           "vt 0.000000 0.000000\n"
           "vt 1.000000 0.000000\n"
           "vt 0.000000 1.000000\n"
           "vt 1.000000 1.000000\n"
           "\n"
           "vn 0.000000 0.000000 1.000000\n"
           "vn 0.000000 1.000000 0.000000\n"
           "vn 0.000000 0.000000 -1.000000\n"
           "vn 0.000000 -1.000000 0.000000\n"
           "vn 1.000000 0.000000 0.000000\n"
           "vn -1.000000 0.000000 0.000000\n"
           "\n"
           "g cube\n"
           "s 1\n"
           "f 1/1/1 2/2/1 3/3/1\n"
           "f 3/3/1 2/2/1 4/4/1\n"
           "s 2\n"
           "f 3/1/2 4/2/2 5/3/2\n"
           "f 5/3/2 4/2/2 6/4/2\n"
           "s 3\n"
           "f 5/4/3 6/3/3 7/2/3\n"
           "f 7/2/3 6/3/3 8/1/3\n"
           "s 4\n"
           "f 7/1/4 8/2/4 1/3/4\n"
           "f 1/3/4 8/2/4 2/4/4\n"
           "s 5\n"
           "f 2/1/5 8/2/5 4/3/5\n"
           "f 4/3/5 8/2/5 6/4/5\n"
           "s 6\n"
           "f 7/1/6 1/2/6 5/3/6\n"
           "f 5/3/6 1/2/6 3/4/6";

}

 static float mapRange(float old_value, float old_bottom, float old_top, float new_bottom, float new_top ) {
     return (old_value - old_bottom) / (old_top - old_bottom) * (new_top - new_bottom) + new_bottom;
 }

//string generateSphere(float radius, int detailLevel) {
//
//    glm::vec3 verts[detailLevel+1][detailLevel+2];
//
//
//    verts[0][0] = glm::vec3(0, radius, 0);
//    for(int i = 1; i < detailLevel; i++){
//        float lon = mapRange(i, 0, detailLevel-1, 0, 2*M_PI);
//
//        for(int j = 0; j < detailLevel; ++j){
//            float lat = mapRange(j, 0, detailLevel-1, 0, M_PI);
//
//            float x = radius *sin(lat) * cos(lon);
//            float y = radius * sin(lat) * sin(lon);
//            float z = radius * cos(lat);
//            verts[i][j] = glm::vec3(x,y,z);
//
//        }
//    }
//
//    std::vector<glm::ivec3> faces;
//
//    for(int i = 0; i < detailLevel; i++) {
//        for(int j = 0; j < detailLevel+1; j++) {
//            int a = i*(detailLevel+2) + j+1;
//            int b = (i+1)*(detailLevel+2) + j+1;
//            int c = i*(detailLevel+2) + j+2;
//            int d = (i+1)*(detailLevel+2) + j+2;
//            faces.push_back(glm::ivec3(a, b, c));
//            faces.push_back(glm::ivec3(b, d, c));}
//    }
//
//    std::ostringstream oss;
//
//    // Write vertices.
//    for (int i = 0; i <= detailLevel; i++) {
//        for (int j = 0; j <= detailLevel+1; j++) {
//            oss << "v " << verts[i][j].x << " " << verts[i][j].y << " " << verts[i][j].z << "\n";
//        }
//    }
//
//    // Write faces.
//    for (const auto &face : faces) {
//        oss << "f " << face.x << " " << face.y << " " << face.z << "\n";
//    }
//
//    return oss.str();
//}


///////////////////////////////////////////////////////////
void setupModels(){



    //    string cubeData = generateCube(2);
//    models.push_back(new Mesh(cubeData, false, moonTex, true));
//    string sphereData = generateSphere(1, 3);
    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\cube.obj)", true, moon, true));
    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\sphere.obj)", true,
                              moon, true));
    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\lucy\lucy.obj)", true,
                              {},true));
//    models.push_back(new Mesh(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\test.obj)", false));
    for (const auto &item: models){
        item->init();
    }
}

char* readShaderFile(const char *fileName) {

        FILE *fp;
        long size = 0;
        char* shaderContent;

        /* Read File to get size */
        fp = fopen(fileName, "rb");
        if(fp == NULL) {
            return "";
        }
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp)+1;
        fclose(fp);

        /* Read File for Content */
        fp = fopen(fileName, "r");
        shaderContent = static_cast<char *>(memset(malloc(size), '\0', size));
        fread(shaderContent, 1, size-1, fp);
        fclose(fp);

        return shaderContent;

}

void setupShaders(){
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    prg = glCreateProgram();

#if defined(SHADER_VIZ_NORMALS)
    const char* vShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\VisualizeNormals.vert)");
    const char* fShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\VisualizeNormals.frag)");
#elif defined(SHADER_TOON)
    const char* vShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\Toon.vert)");
    const char* fShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\Toon.frag)");
#elif defined(SHADER_GOURAUD)
    const char* vShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\Gouraud.vert)");
    const char* fShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\Gouraud.frag)");
#elif defined(SHADER_PHONG)
    const char* vShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\Phong.vert)");
    const char* fShader = readShaderFile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Shaders\Phong.frag)");
#endif
    glShaderSource(vs, 1, &vShader, nullptr);
    glCompileShader(vs);
    glShaderSource(fs, 1, &fShader, nullptr);
    glCompileShader(fs);


    glAttachShader(prg, vs);
    glAttachShader(prg, fs);

    GLint isLinked = 0;
    GLint maxLength = 0;
    char* infoLog = static_cast<char *>(malloc(1024));

    glLinkProgram(prg);

    glGetProgramiv(prg, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE) {
        printf("Shader Program Linker Error\n");

        glGetProgramiv(prg, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(prg, maxLength, &maxLength, &infoLog[0]);

        printf("%s\n", infoLog);

        glDeleteProgram(prg);

        glDeleteShader(vs);
        glDeleteShader(fs);
        free(infoLog);

        cout<<"we sad"<<endl;
        return;
    }



    glUseProgram(prg);
    GLint  lightLocation = glGetUniformLocation(prg, "lightSource");
    GLint  ambientLocation = glGetUniformLocation(prg, "ambientColor");
    GLint  diffuseLocation = glGetUniformLocation(prg, "diffuseColor");
    GLint  specularLocation = glGetUniformLocation(prg, "specularColor");

    glUniform4fv(lightLocation, 1, glm::value_ptr(lightDirection));
    glUniform4fv(ambientLocation, 1, glm::value_ptr(ambient));
    glUniform4fv(diffuseLocation, 1, glm::value_ptr(diffuse));
    glUniform4fv(specularLocation, 1, glm::value_ptr(specular));




}
// Setup the rendering state
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
        angleY += angleChangeSpeed;

    else if(key == GLUT_KEY_RIGHT )
        angleY -= angleChangeSpeed;

    else if (key == GLUT_KEY_UP)
        angleX += angleChangeSpeed;

    else if(key == GLUT_KEY_DOWN )
        angleX -= angleChangeSpeed;



    cout <<xTranslation<< endl;
    cout <<yTranslation<< endl;
    glutPostRedisplay();

}

void KeyboardFunc(unsigned char key, int x, int y) {

    if (key == 'd')
        xTranslation-= translationFactor;
    else if(key == 'a')
        xTranslation+= translationFactor;
    if (key == 'w')
        yTranslation-= translationFactor;
    else if(key == 's')
        yTranslation+= translationFactor;

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
                if (scale > 0.01) {
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

