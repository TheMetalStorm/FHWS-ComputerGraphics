#pragma once
#define GLUT_DISABLE_ATEXIT_HACK

#include <Windows.h>		// Must have for Windows platform builds
#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

#include "freeglut.h"			// Glut (Free-Glut on Windows)
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

using namespace std;
using namespace Eigen;

typedef Vector3f RGBPixel ;
//struct RGBPixel {
//	GLfloat R, G, B;
//	RGBPixel(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0) : R(r), G(g), B(b) {};
//};


struct PassiveEdge {
	int ymin, xmin, ymax, xmax;
	PassiveEdge(int ymin = 0, int xmin = 0, int ymax = 0, int xmax = 0) : ymin(ymin), xmin(xmin), ymax(ymax), xmax(xmax) {}
};

struct ActiveEdge {
	int ymax;
	float dx, xs;
	ActiveEdge(float xs = 0, float dx = 0, int ymax = 0) : xs(xs), dx(dx), ymax(ymax) {}
};

struct Obj{
    //v
    vector<glm::vec3> verts = {};
    //vt
    vector<glm::vec2> vertTex= {};
    //vn
    vector<glm::vec3> vertNormals= {};
    //f
    vector< unsigned int > vertexIndices, uvIndices, normalIndices;


    Obj() = default;
};


const int RESOLUTION = 800;

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);
