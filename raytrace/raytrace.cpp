// raytrace.cpp : Defines the entry point for the console application.
//


#include "stdafx.h" 
#include "Ray.h"
#include "Color.h"
#include "Image.h"
#include "vector"
#include "Camera.h"

const double SCREENWIDTH = 1000;
const double SCREENHEIGHT = 1000;
double PI = 3.14159265358979323846264338327950288;



using namespace std;

vector<Surface> surfaces;
vector<Property> properties;
vector<Objekt> objekte;
vector<Light> lights;
int Xresolution;
int Yresolution;
double eyeX;
double eyeY;
double eyeZ;
double fov;
double aspect;
Vector up;
Vector lookAt;
Color bg;


extern "C" {
	extern FILE *yyin;
	int yyparse();

	void add_light(char *n, double dirx, double diry, double dirz, double colr, double colg, double colb) {
		fprintf(stderr,"  adding light %f %f %f %f %f %f\n", dirx, diry, dirz, colr, colg, colb);
		lights.push_back(Light(Vector(dirx,diry,dirz).normalize(), Color(colr, colg, colb)));
	};
	void add_quadric(char *n, double a, double b, double c, double d, double e, double f, double g, double h, double j, double k) {
		fprintf(stderr,"  adding quadric %s %f %f %f %f %f %f %f %f %f %f\n", n, a,b,c,d,e,f,g,h,j,k);
		surfaces.push_back(Surface(n, a,b,c,d,e,f,g,h,j,k));
	};
	void add_property(char *n,  double ar, double ag, double ab, double r, double g, double b, double s, double m) {
		fprintf(stderr,"  adding prop %f %f %f %f %f\n", r, g, b, s, m);
		properties.push_back(Property(n, Color(ar, ag, ab), Color(r, g, b), s, m));
	};
	void add_resolution(int x, int y) {
		Xresolution = x;
		Yresolution = y;
		fprintf(stderr, "  adding resolution X: %d, Y: %d\n", x,y);
	}

	void add_background_color(double r, double g, double b) {
		bg.r = r;
		bg.g = g;
		bg.b = b;
		fprintf(stderr, "  adding BG Color R: %f, G: %f, B: %f\n", r, g, b);
	};


	void add_eyepoint(double x, double y, double z) {
		eyeX = x;
		eyeY = y;
		eyeZ = z;
		fprintf(stderr, "  adding eyepoint X: %f, Y: %f, Z: %f\n", x, y, z);
	};

	void add_look_at(double x, double y, double z) {
		lookAt.x = x;
		lookAt.y = y;
		lookAt.z = z;
		fprintf(stderr, "  adding lookatpoint X: %f, Y: %f, Z: %f\n", x, y, z);
	};

	void add_aspect(double a) {
		aspect = a;
		fprintf(stderr, "  adding aspect %f\n", a);
	};

	void add_fovy(double fovy) {
		fov = fovy;
		fprintf(stderr, "  adding fov %f\n", fovy);
	};

	void add_up(double x, double y, double z) {
		up.x = x;
		up.y = y;
		up.z = z;
		fprintf(stderr, "  adding Up Vector X: %f, Y: %f, Z: %f\n", x, y, z);

	};

	void add_objekt(char *ns, char *np) {
		Surface *s = NULL;
		Property *p = NULL;
		string ss(ns);
		string sp(np);

		for(vector<Surface>::iterator i = surfaces.begin(); i != surfaces.end(); ++i) 
			if(i->getName() == ss) {
				s = &(*i);
				break;
			}
		for(vector<Property>::iterator i = properties.begin(); i != properties.end(); ++i) 
			if(i->getName() == sp) {
				p = &(*i);
				break;
			}

		if(s == NULL) {
			fprintf(stderr, "Surface not found: %s\n", ns);
			exit(1);
		}
		if(p == NULL) {
			fprintf(stderr, "Property not found: %s\n", np);
			exit(1);
		}
		objekte.push_back(Objekt(s, p));
		fprintf(stderr, "  adding object: surface %s, property %s\n", ns, np);
	}
}

int main(int argc, _TCHAR* argv[])
{
	/* parse the input file */
	yyin = fopen("data/dflt.data","r");
	if(yyin == NULL) {
		fprintf(stderr, "Error: Konnte Datei nicht �ffnen\n");
		return 1;
	}
	yyparse();
	fclose (yyin);
	

	double dx = SCREENWIDTH / (double)Xresolution * aspect * tan(fov * PI / 180 / 2);
	double dy = SCREENHEIGHT / (double)Yresolution * tan(fov * PI / 180 / 2);
	double y = -0.5 * SCREENHEIGHT * tan(fov * PI / 180 / 2);
	Vector eye(eyeX, eyeY, eyeZ);

	Camera cam(eye, lookAt, up);
	Ray	ray(Vector(1,0,0), cam.eye ,0);

	Image bild(Xresolution, Yresolution);

	for (int scanline=0; scanline < Yresolution; scanline++) {

		printf("%4d\r", Yresolution-scanline);
		y += dy;
		double x = -0.5 * SCREENWIDTH * aspect * tan(fov * PI / 180 / 2);

		for (int sx=0; sx < Xresolution; sx++) {

			//Vector v;
			//v.x = 2 * sx * tan(fov * PI / 180 / 2 )/Xresolution;
			//v.y = 2 * scanline * tan(fov * PI / 180  / 2)/ Yresolution;
			//v.z = 1;
			//v = v.normalize();

			int distanceToViewingPlane = 1;
			Vector pixelPos = cam.getRight().svmpy(x).vsub(cam.getActualUp().svmpy(y)).vadd(cam.getForward().svmpy(1));
			ray.setDirection(pixelPos.vsub(ray.getOrigin()).normalize());
			//ray.setDirection(Vector(x, y, 0.0).vsub(ray.getOrigin()).normalize()); //look at current pixel
			x += dx;
			Color color = ray.shade(objekte, lights, bg);

			bild.set(sx, scanline,
				color.r > 1.0 ? 255 : int(255 * color.r),
				color.g > 1.0 ? 255 : int(255 * color.g),
				color.b > 1.0 ? 255 : int(255 * color.b));
		}
	}

	char *name = "raytrace-bild.ppm";
	bild.save(name);

	return 0;
}

