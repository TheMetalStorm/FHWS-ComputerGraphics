// 2D
// Simple framework for 2D-Drawing

#define FREEGLUT_STATIC

#include "simple.h"
#include "render.h"
#include "ppm.h"
#include <string>

using namespace std;

RGBPixel framebuffer[RESOLUTION][RESOLUTION];


void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
	framebuffer[x < 0 ? 0 : (x >= RESOLUTION ? RESOLUTION - 1 : x)][y < 0 ? 0 : (y >= RESOLUTION ? RESOLUTION - 1 : y)] =
		RGBPixel(r, g, b);
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat d = 2.0f / GLfloat(RESOLUTION);
	for (int y = 0; y < RESOLUTION; y++)		
		for (int x = 0; x < RESOLUTION; x++) {
			const RGBPixel& p = framebuffer[x][y];
			if (p.R == 0 && p.G == 0 && p.B == 0)
				continue;
			glColor3f(p.R, p.G, p.B);
			GLfloat vpx = GLfloat(x) * 2.0f / GLfloat(RESOLUTION) - 1.0f;
			GLfloat vpy = GLfloat(y) * 2.0f / GLfloat(RESOLUTION) - 1.0f;
			glRectf(vpx, vpy, vpx + d, vpy + d);
		}

	// Flush drawing commands
    glFlush();
}

///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void KeyPress(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': exit(1);
	}
}




///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
 	glutCreateWindow("2D-Test");
	glutReshapeWindow(RESOLUTION, RESOLUTION);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(KeyPress);

	SetupRC();
////////// put your framebuffer drawing code here /////////////

	//Uebung 1
	
	//2a)
	//lineBresenheimFirstOctant(int2(250, 250), int2(500, 400), 1, 0, 0);
	//2b)
	//lineBresenheim(int2(RESOLUTION/2, RESOLUTION / 2), int2(700, 500), 0, 1, 0);
	//lineBresenheim(int2(0, 0), int2(600, 200), 0, 1, 0);

	//2c)
	lineMidpoint(int2(0, 200), int2(200, 250), 0, 0, 1);

	//Uebung 2
	//1)
	circleMidpoint(400, 500, 50, .4, .6, 0);
	
	//3
	vector<int2> points{

		int2(100,100),
		int2(100,200),
		int2(200,300),
		int2(400,100),
		int2(300,600),
		int2(400,500)
	};
	
	/*vector<int2> test{

		int2(100,100),
		int2(100,400),
		int2(400,400),
		int2(400,100)
	};*/

	//bezier(points, 1, 0, 0); 
		
	//TODO: 4) bsplines


	//Uebung 2
	//1)
	//a)
	//paralelRect(int2(300, 500), int2(500, 300), .5, .5, 0);
	//b
	//triangle(int2(100, 100), int2(300, 300), int2(50, 500), 1, 0, 0);
	//c
	polygon(points, 0, 0, 1);
	framebufferToPPM(framebuffer, "export");

/////////////////////////////////
	glutMainLoop();

    return 0;
}




