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

#include "simple.h"
#include "Obj.h"
#include <math.h>
#include <iostream>

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat angleX = 0.0f;
GLfloat angleZ = 0.0f;
GLfloat angleChangeSpeed = 5;
GLfloat scale = 0.0f;

float scaleFactor = 0.05;

GLfloat xstep = 0.03f;
GLfloat ystep = 0.02f;

Obj cubeModel = Obj(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\cube.obj)");
GLuint prg;
///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    //a
//    drawRect(0,0);
    //b
//    drawRect(x,y);
    //c)
//    drawMovingRectTranslate(x, y);
    //d
//    drawMovingRectTranslateRotateScale( x,  y);

    GLfloat rotationAngle = angleX;
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    GLint rotationUniformLocation = glGetUniformLocation(prg, "rotation");
    glUniformMatrix4fv(rotationUniformLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

    //FIXME: falls wir weniger indices haben (keine vt/vn), funktioniert cubeModels.indices.size/3 nicht mehr
    glDrawArrays(GL_TRIANGLES, 0, cubeModel.getIndicesCount()/3);
    cout<<cubeModel.getIndicesCount() << endl;
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
// Setup the rendering state
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    GLenum err = glewInit();


    const GLchar* vShader  = "#version 330\n"
                             "\n"
                             "layout(location = 0)in vec3 vert;\n"
                             "layout(location = 1)in vec2 uv;\n"
                             "layout(location = 2)in vec3 normal;\n"
                             "\n"
                             "out vec3 Normal;\n"
                             "\n"
                             "uniform mat4 rotation;\n"
                             "\n"
                             ""
                             "void main()\n"
                             "{\n"
                             "    Normal = normal;\n  "
                             "    gl_Position = rotation * vec4(vert, 1.0);\n"
                             "}";

    const char *fragmentShader1Source = "#version 330 core\n"
                                        "in vec3 Normal;\n"
                                        "out vec4 fragColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "    fragColor = vec4(0.5 + 0.5 * gl_FragCoord.xyz, 1.0);\n"
                                        "}\n\0";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // the first fragment shader that outputs the color orange
    prg = glCreateProgram();

    glShaderSource(vs, 1, &vShader, nullptr);
    glCompileShader(vs);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);

    glAttachShader(prg, vs);
    glAttachShader(prg, fragmentShaderOrange);

    glLinkProgram(prg);
    glUseProgram(prg);

    auto* vertexData = cubeModel.getVertexDataFromModel();

    GLuint vbo = 0, vao = 0;

// Draw the object

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cubeModel.getIndicesCount() * ( 2 + 3), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // siehe Shader: „location 0“
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8,
                          (const GLvoid*)(sizeof(GLfloat)*3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8,
                          (const GLvoid*)(sizeof(GLfloat)*5));

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    delete[] vertexData;
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void TimerFunction(int value)
{
    if(x > 1 - 0.25 || x < -1+0.25)
        xstep = -xstep;
    if(y > 1 - 0.25 || y < -1+0.25)
        ystep = -ystep;
    x += xstep;
    y += ystep;

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

void KeyboardFunc(unsigned char key, int x, int y) {

    if (key == 'd')
        angleZ += angleChangeSpeed;
    else if(key == 'a')
        angleZ -= angleChangeSpeed;
    if (key == 'w')
        angleX += angleChangeSpeed;
    else if(key == 's')
        angleX -= angleChangeSpeed;

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
    glutKeyboardFunc(KeyboardFunc);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

	SetupRC();

	glutMainLoop();

    return 0;
}

