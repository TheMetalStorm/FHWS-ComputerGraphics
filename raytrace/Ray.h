#pragma once
#include "Vector.h"
#include "Color.h"
#include "Light.h"
#include "Objekt.h"
#include "vector"

class Ray
{
	Vector direction, origin;
	int	 depth;

	Color shaded_color(Light *light, Ray &reflectedray, Vector &normal, Objekt *obj);

public:
	Ray(void) : direction(), origin(), depth (0) {};
	Ray(Vector &dir, Vector &orig, int d) : direction(dir), origin(orig), depth (d) {};
	Color shade(std::vector<Objekt> &, std::vector<Light> &, Color bg);

	Vector getDirection() { return direction; };
	Vector getOrigin() { return origin; };
	int getDepth() { return depth; };

	void setDirection(Vector &v) { direction=v; };
	void setOrigin(Vector &v) { origin=v; };
	void setDepth(int d) { depth=d; };

	Ray reflect(Vector &origin, Vector &normal);
	double intersect(Objekt &obj);

};


