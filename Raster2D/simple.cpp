// 2D
// Simple framework for 2D-Drawing

#define FREEGLUT_STATIC

#include "simple.h"
#include "render.h"
#include "ppm.h"
#include <string>

using namespace std;
using namespace Eigen;

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
//	lineBresenheimFirstOctant(Vector2i(250, 250), Vector2i(500, 400), 1, 0, 0);
	//2b)
//	lineBresenheim(Vector2i(0, 400), Vector2i(600, 200), 0, 1, 0);
	//2c)
//	lineMidpoint(Vector2i(0, 200), Vector2i(200, 250), 0, 0, 1);

	//Uebung 2
	//1)
//	circleMidpoint(400, 500, 50, .4, .6, 0);
	
	//3
	vector<Vector2i> points{

            Vector2i(100,100),
            Vector2i(100,200),
            Vector2i(200,300),
            Vector2i(400,100),
            Vector2i(300,600),
            Vector2i(400,500),

    };

    vector<Vector2i> a{

            Vector2i(100,100),
            Vector2i(100,300),
            Vector2i(300,300),
            Vector2i(300,100),
            Vector2i(500,100),
            Vector2i(500,500),
            };

	vector<Vector2i> square{

		Vector2i(100,100),
		Vector2i(100,400),
		Vector2i(400,400),
		Vector2i(400,100)
	};
    vector<Vector2i> trig{

            Vector2i(100,100),
            Vector2i(200,100),
            Vector2i(200,200)
    };

//	bezier(square, 1, 0, 0);
    vector<float> knots = {0.1, 0.2, 0.3, 0.4, 0.49, 0.75, 1, 1, 1};

	//FIXME: 4) bsplines
    //bspline(points, knots, 0,1,0);

	//Uebung 2
	//1)
	//a)
//	paralelRect(Vector2i(300, 500), Vector2i(500, 300), .5, .5, 0);
	//b
//	triangle(Vector2i(100, 100), Vector2i(300, 300), Vector2i(50, 500), 1, 0, 0);
	//c

    //FIXME
//	polygon(points, 0, 0, 1);

    catmullRom(a, 1,1,1);



	framebufferToPPM(framebuffer, "withoutOutline");

/////////////////////////////////
	glutMainLoop();

    return 0;
}






