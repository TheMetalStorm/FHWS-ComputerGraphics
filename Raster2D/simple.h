#pragma once
#define GLUT_DISABLE_ATEXIT_HACK

#include <Windows.h>		// Must have for Windows platform builds
#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "glut.h"			// Glut (Free-Glut on Windows)
#include <cmath>
#include <vector>

using namespace std;

struct RGBPixel {
	GLfloat R, G, B;
	RGBPixel(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0) : R(r), G(g), B(b) {};
};

struct float2 {
	GLfloat x, y;
	float2(GLfloat x = 0, GLfloat y = 0) : x(x), y(y){};
};

struct int2 {
	GLint x, y;
	int2(GLint x = 0, GLint y = 0) : x(x), y(y) {};
};

struct PassiveEdge {
	int ymin, xmin, ymax, xmax;
	PassiveEdge(int ymin = 0, int xmin = 0, int ymax = 0, int xmax = 0) : ymin(ymin), xmin(xmin), ymax(ymax), xmax(xmax) {}
};

struct ActiveEdge {
	int ymax;
	float dx, xs;
	ActiveEdge(float xs = 0, float dx = 0, int ymax = 0) : xs(xs), dx(dx), ymax(ymax) {}
};



const int RESOLUTION = 600;

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);
