#pragma once
#include "simple.h"
#include <iostream>
#include <algorithm>

using namespace std;


void lineMidpoint(int2 p0, int2 p1, GLfloat r, GLfloat g, GLfloat b) {
	
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;

	int x = x0;
	int y = y0;
	const int dx = x1 - x0;
	const int dy = y1 - y0;
	int f = dy - dx / 2;
	for (int i = 1; i < dx; i++) {
		setPixel(x, y, r, g, b);
		x++;
		if (f > 0) {
			y += 1;
			f -= dx;
		}
		f += dy;
	}
}

void circleMidpoint(int x, int y, int radius, GLfloat r, GLfloat g, GLfloat b) {
	int x1 = 0;
	int y1 = radius;
	int f = 1 - radius;
	int dx = 3;
	int dy = 2 - 2 * radius;
	while (x1 <= y1) {
		setPixel(x + x1, y + y1, r, g, b);
		setPixel(x + x1, y - y1, r, g, b);
		setPixel(x - x1, y - y1, r, g, b);
		setPixel(x - x1, y + y1, r, g, b);
		setPixel(x + y1, y - x1, r, g, b);
		setPixel(x + y1, y + x1, r, g, b);
		setPixel(x - y1, y - x1, r, g, b);
		setPixel(x - y1, y + x1, r, g, b);
		x1++;
		if (f > 0) {
			y1--;
			f += dy;
			dy += 2;
		}
		f += dx;
		dx += 2;
	}
}

void lineBresenheimFirstOctant(int2 p0, int2 p1, GLfloat r, GLfloat g, GLfloat b) {
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;

	const int dx = x1 - x0;
	const int dy = y1 - y0;
	const int const1 = dx + dx;
	const int const2 = dy + dy;
	int error = const2 - dx;
	while (x0 <= x1) {
		setPixel(x0, y0, r, g, b);
		x0++;
		if (error <= 0)
			error += const2;
		else {
			y0++;
			error += const2 - const1;
		}
	}
}


int sgn(int x) {
	if (x > 0) return 1;
	else if (x == 0) return 0;
	else return -1;
}

//Src: https://www.uobabylon.edu.iq/eprints/publication_2_22893_6215.pdf
void lineBresenheim(int2 p0, int2 p1, GLfloat r, GLfloat g, GLfloat b) {
	
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;

	int x = x0;
	int y = y0;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	const int s1 = sgn(x1 - x0);
	const int s2 = sgn(y1 - y0);

	int interchange;

	if (dy > dx) {
		int t = dx;
		dx = dy;
		dy = t;
		interchange = 1;
	}
	else {
		interchange = 0;
	}

	int e = 2 * dy - dx;
	const int a = 2 * dy;
	const int ba = 2 * dy - 2 * dx;
	setPixel(x, y, r, g, b);

	for (int i = 1; i < dx; i++) {
		if (e < 0) {
			if (interchange == 1) {
				y = y + s2;
			}
			else {
				x = x + s1;
			}
			e = e + a;
		}
		else {
			y = y + s2;
			x = x + s1;
			e = e + ba;
		}
		setPixel(x, y, r, g, b);
	}
}

int2 deCasteljau(const vector<int2>& points, float u) {
	
	const int n = points.size();
	vector <float2> temp;

	for (int i = 0; i < n; i++) {
		temp.push_back(float2(points[i].x, points[i].y));
	}

	for (int k = 1; k < n; k++) {
		for (int i = 0; i < (n - k); i++) {

			temp[i] = {
				(1.0f - u) * temp[i].x + u * temp[i + 1].x,
				(1.0f - u) * temp[i].y + u * temp[i + 1].y };
		}
	}
		
	return int2((int)temp[0].x, (int)temp[0].y);
}

void bezier(vector<int2>& points, GLfloat r, GLfloat g, GLfloat b) {

	const int iterations = 1000;
	const float increment = 1.0 / iterations;

	for (int i = 0; i <= iterations; i++) {
		const float u = i * increment;
		const int2 point = deCasteljau(points, u);
		setPixel(floor(point.x), floor(point.y), r, g, b);
	}
}

void paralelRect(int2 p0, int2 p1, GLfloat r, GLfloat g, GLfloat b) {

	const int x0 = p0.x;
	const int y0 = p0.y;
	const int x1 = p1.x;
	const int y1 = p1.y;

	for (int y = y0; y > y1; y--) {
		for (int x = x0; x <= x1; x++) {
			setPixel(x, y, r, g, b);

		}
	}
}

void triangle(int2 p0, int2 p1, int2 p2, GLfloat r, GLfloat g, GLfloat b) {

	const int x0 = p0.x;
	const int y0 = p0.y;
	
	const int x1 = p1.x;
	const int y1 = p1.y;

	const int x2 = p2.x;
	const int y2 = p2.y;

	const int xmin = min(x0, min(x1, x2));
	const int ymin = min(y0, min(y1, y2));
	const int xmax = max(x0, max(x1, x2));
	const int ymax = max(y0, max(y1, y2));
	
	int f0 = (y0 - y1) * (xmin - x0) + (x1 - x0) * (ymin - y0);
	int f1 = (y1 - y2) * (xmin - x1) + (x2 - x1) * (ymin - y1);
	int f2 = (y2 - y0) * (xmin - x2) + (x0 - x2) * (ymin - y2);
	for (int y = ymin; y <= ymax; y++) {
		int ff0 = f0, ff1 = f1, ff2 = f2;
		for (int x = xmin; x <= xmax; x++) {
			if (ff0 >= 0 && ff1 >= 0 && ff2 >= 0)
				setPixel(x, y, r, g, b);
			ff0 = ff0 + (y0 - y1);
			ff1 = ff1 + (y1 - y2);
			ff2 = ff2 + (y2 - y0);
		}
		f0 = f0 + (x1 - x0);
		f1 = f1 + (x2 - x1);
		f2 = f2 + (x0 - x2);
	}
}

bool compareActiveEdge(const ActiveEdge& a, const ActiveEdge& b) {
	return a.xs < b.xs;
}

void polygon(const vector<int2>& points, GLfloat r, GLfloat g, GLfloat b) {
	int minY = 2147483647;
	int maxY = 0;

	vector<PassiveEdge> passive;

	for (int i = 0; i < points.size(); i++) {

		PassiveEdge e;
		int2 a, b;
		if (i == 0) {
			a = points[points.size() - 1];
			b = points[0];
		}
		else {
			a = points[i - 1];
			b = points[i];
		}

		e.ymin = a.y < b.y ? a.y : b.y;
		e.xmin = a.x < b.x ? a.x : b.x;
		e.ymax = a.y >= b.y ? a.y : b.y;
		e.xmax = a.x >= b.x ? a.x : b.x;

		passive.push_back(e);
				
	}

	//NOTE: evtl müssen alle kannten nach ymin sortiert werden?

	for (int2 p : points) {
		if (p.y < minY) {
			minY = p.y;
		}
		if (p.y > maxY) {
			maxY = p.y;
		}
	}


	for (int y = minY; y <= maxY; y++) {
		// 1) Verschiebe Kanten mit ymin = y von passiver in aktive Liste
		// ???: sortieren nach ymin
		vector<ActiveEdge> active;
		for (PassiveEdge e : passive) {
			//Ignoriert rein horizontale Kanten (ymin = ymax)
			if (y == e.ymin && e.ymin != e.ymax) {
				ActiveEdge toAdd;
				toAdd.ymax = e.ymax;
				toAdd.xs = e.xmin;
				toAdd.dx = (float)(e.xmax - e.xmin) / (float)(e.ymax - e.ymin);
				active.emplace_back(toAdd);
			}
		}
		
		// 3) Entferne Kanten mit ymax = y aus aktiver Liste
		for (vector<ActiveEdge>::iterator it = active.begin();it != active.end();) {

			if (y == it->ymax) {
				it = active.erase(it);
			}
			else {
				++it;
			}
		}

		//4. Sortiere Kanten der aktiven Liste nach xs
		sort(active.begin(), active.end(), compareActiveEdge);

		//5. Setze Pixel innerhalb der Spans
		for (int i = 0; i < active.size();i++) {
			if (i % 2 == 0) {
				for (int x = active[i].xs; x <= active[i + 1].xs; i++) {
					setPixel(x, y, r, g, b);
				}
			}
		}


		//6. Inkrementiere y um 1 und alle xs der aktiven Liste um ∆x
		//y++ in for-loop
		for (ActiveEdge e : active) {
			e.xs += e.dx;
		}

	}

}




