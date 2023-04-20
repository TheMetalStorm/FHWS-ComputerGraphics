#pragma once
#include "simple.h"
#include "transformation.h"
#include <iostream>
#include <algorithm>
#include <utility>
#include <bits/stdc++.h>


const int INSIDE = 0; // 0000
const int LEFT = 1; // 0001
const int RIGHT = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8; // 1000




int Outcode(double x, double y, int x_min, int y_min, int x_max, int y_max)
{
    // initialized as being inside
    int code = INSIDE;

    if (x < x_min) // to the left of rectangle
        code |= LEFT;
    else if (x > x_max) // to the right of rectangle
        code |= RIGHT;
    if (y < y_min) // below the rectangle
        code |= BOTTOM;
    else if (y > y_max) // above the rectangle
        code |= TOP;

    return code;
}

//src: https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
bool getClippedPoints(const Vector2i& oldP0, const Vector2i& oldP1, Vector2i *newP0, Vector2i *newP1, int x_min, int y_min, int x_max, int y_max){


    double x1 = oldP0.x();
    double y1 = oldP0.y();
    double x2 = oldP1.x();
    double y2 = oldP1.y();
    int code1 = Outcode(oldP0.x(), oldP0.y(), x_min, y_min, x_max, y_max);
    int code2 = Outcode(oldP1.x(), oldP1.y(), x_min, y_min, x_max, y_max);

    bool accept = false;
    while(true){
        if ((code1 == 0) && (code2 == 0)){
            accept = true;
            break;
        }
        else if( code1 & code2){
            break;
        }
        else{
            int code_out;
            double x, y;

            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            if (code_out & TOP) {
                // point is above the clip rectangle
                x = x1 + (x2 - x1) * ((float)y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM) {
                // point is below the rectangle
                x = x1 + (x2 - x1) * ((float)y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT) {
                // point is to the right of rectangle
                y = y1 + (y2 - y1) * ((float)x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT) {
                // point is to the left of rectangle
                y = y1 + (y2 - y1) * ((float)x_min - x1) / (x2 - x1);
                x = x_min;
            }

            // Now intersection point x, y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = Outcode(x1, y1, x_min, y_min, x_max, y_max);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = Outcode(x2, y2, x_min, y_min, x_max, y_max);
            }

        }
    }
    if(accept){
        *newP0 = {x1, y1};
        *newP1 = {x2, y2};
    }
    else{
        *newP0 = {};
        *newP1 = {};
    }

    return accept;
}

void lineMidpoint(const Vector2i& p0, const Vector2i&  p1, GLfloat r, GLfloat g, GLfloat b) {

    Vector2i temp0, temp1;
    bool passed = getClippedPoints(p0, p1, &temp0, &temp1, 0, 0, RESOLUTION, RESOLUTION);

    if(passed){
        int x0 = temp0.x();
        int y0 = temp0.y();
        int x1 = temp1.x();
        int y1 = temp1.y();

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

void lineBresenheimFirstOctant(Vector2i p0, Vector2i p1, GLfloat r, GLfloat g, GLfloat b) {
	int x0 = p0.x();
	int y0 = p0.y();
	int x1 = p1.x();
	int y1 = p1.y();

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
void lineBresenheim(Vector2i p0, Vector2i p1, GLfloat r, GLfloat g, GLfloat b) {
	
	int x0 = p0.x();
	int y0 = p0.y();
	int x1 = p1.x();
	int y1 = p1.y();

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

Vector2i deCasteljau(const vector<Vector2i>& points, float u) {
	
	const int n = points.size();
	vector <Vector2f> temp;

	for (int i = 0; i < n; i++) {
		temp.emplace_back(points[i].x(), points[i].y());
	}

	for (int k = 1; k < n; k++) {
		for (int i = 0; i < (n - k); i++) {

			temp[i] = {
				(1.0f - u) * temp[i].x() + u * temp[i + 1].x(),
				(1.0f - u) * temp[i].y() + u * temp[i + 1].y() };
		}
	}

	return {(int)floor(temp[0].x()), (int)ceil(temp[0].y())};

//    const int n = points.size();
//    vector<vector<float2>> b(n+1,vector<float2>(n+1));
//
//    for (int i = 0; i<= n;i++){
//        b[i][0] = {(float)points[i].x,(float)points[i].y};
//    }
//
//    for (int j = 1;j<=n;j++){
//        for (int i = j; i<=n;i++){
//            b[i][j] = {
//				(1.0f - u) * b[i-1][j-1].x + u * b[i][j-1].x,
//				(1.0f - u) * b[i-1][j-1].y + u * b[i][j-1].y };
//        }
//
//    }
//
//    return {(int)floor(b[n][n].x), (int)floor(b[n][n].y)};
}

void bezier(vector<Vector2i>& points, GLfloat r, GLfloat g, GLfloat b) {

	const int iterations = 1000;
	const float increment = 1.0 / iterations;

	for (int i = 0; i <= iterations; i++) {
		const float u = (float)i * increment;
		const Vector2i point = deCasteljau(points, u);
		setPixel(point.x(), point.y(), r, g, b);

	}
}


int findIndex(vector<float> v, float f) {
    for (int i = v.size() - 1; i >= 0; i--) {
        if (v[i] <= f) {
            return i;
        }
    }
    return 0;
}

Vector2i openbspline(const vector<Vector2f> &points, const vector<float> &knots, int n, float t) {

    int i = findIndex(knots, t);

    vector<vector<Vector2f>> b(100, vector<Vector2f>(100));

    for (int j = 0; j <= n; j++) {
        for (int l = i - n + j; l <= i ; l++) {
            if(j==0){
                b[l][0] = points[l];
            }
            else{
                float tjl = (t - knots[l]) / (knots[l + n + 1 - j] - knots[l]);
                b[l][j].y() = (1.0f - tjl) * b[l - 1][j - 1].y() + tjl * b[l][j - 1].y();
                b[l][j].x() = (1.0f - tjl) * b[l - 1][j - 1].x() + tjl * b[l][j - 1].x();
            }
        }
    }

    return {b[i][n].x(), b[i][n].y()};
}


//fixme
Vector2i closedbspline(const vector<Vector2f> &points, const vector<float> &knots, int n, float t) {
    int i = findIndex(knots, t);

    int m = points.size()-1;
    vector<vector<Vector2f>> b(100, vector<Vector2f>(100));

    for (int j = 0; j <= n; j++) {
        int l = i-n+j-1;
        do{
            l +=1;
            if(l<0){
                l = l+m+1;
                t = t-knots[0]+knots[m+1];
            }
            else{
                if(l>=m+1){
                    l = l-m-1;
                    t=t-knots[m+1]+knots[0];
                }
            }
            if(j == 0){
                b[l][0] = points[l];
            }
            else{
                float tjl = (t - knots[l]) / (knots[l + n + 1 - j] - knots[l]);

                b[l][j].y() = (1.0f - tjl) * b[(l - 1)%(m+1)][j - 1].y() + tjl * b[l][j - 1].y();
                b[l][j].x() = (1.0f - tjl) * b[(l - 1)%(m+1)][j - 1].x() + tjl * b[l][j - 1].x();
            }
        }
        while(l!=i);
    }

    return {b[i][n].x(), b[i][n].y()};
}


void bspline(const vector<Vector2i>& points, const vector<float>& knots, bool closed, GLfloat r, GLfloat g, GLfloat b){
    int degree = 2;
    int numKnots = knots.size();
    int numIntervals = numKnots - degree - 1;
    int numPoints = 2000; // Number of points to generate on the curve
    double tMin = knots[degree];
    double tMax = knots[numIntervals];
    double tStep = (tMax - tMin) / (numPoints - 1);

    const int s = points.size();
    vector <Vector2f> temp;

    for (int i = 0; i < s; i++) {
        temp.emplace_back(points[i].x(), points[i].y());
    }

    if(closed){
        //FIXME: crash :(
        for (int i = degree; i < numIntervals; i++) {
            for (int j = 0; j < numPoints; j++) {
                double t = tMin + j * tStep;
                Vector2i point = closedbspline(temp, knots, degree, t);
                setPixel(point.x(), point.y(), r,g,b);
            }
        }
    }

    else{
        for (int i = degree; i < numIntervals; i++) {
            for (int j = 0; j < numPoints; j++) {
                double t = tMin + j * tStep;
                Vector2i point = openbspline(temp, knots, degree, t);
                setPixel(point.x(), point.y(), r,g,b);
            }
        }
    }

}

void paralelRect(Vector2i p0, Vector2i p1, GLfloat r, GLfloat g, GLfloat b) {

    //clipping for paralelRect >_>
	const int x0 = p0.x();// < 0 ? 0 : p0.x();
	const int y0 = p0.y();//> RESOLUTION ? RESOLUTION : p0.y();
	const int x1 = p1.x();// > RESOLUTION ? RESOLUTION : p1.x();
	const int y1 = p1.y();//<0? 0:p1.y();

    for (int y = y0; y >= y1; y--){
        lineMidpoint({x0,y},{x1,y}, r,g,b);
    }
}

void paralelRect(Vector3i p0, Vector3i p1, GLfloat r, GLfloat g, GLfloat b) {
    paralelRect(Vector2i{p0.x(), p0.y()},Vector2i{p1.x(), p1.y()}, r,g,b);
}

Vector3f getBarycentricCoordiantes(const Vector2i& a, const Vector2i& b, const Vector2i& c, int x, int y) {
    float w1, w2,  w3;

    w1 = (((float)a.x() * ((float)c.y() - (float)a.y()) + ((float)y - (float)a.y()) * ((float)c.x() - (float)a.x()) - (float)x * (float)(c.y() - a.y()))) /
         (float)((b.y() - a.y()) * (c.x() - a.x()) - (b.x() - a.x()) * (c.y() - a.y()));

    w2 = ((float)y - (float)a.y() - w1 * ((float)b.y() - (float)a.y())) / ((float)c.y() - (float)a.y());

    w3 = 1.0f-w1-w2;

    return {w3, w1, w2};
}

RGBPixel mixColor(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, const RGBPixel& c0, const RGBPixel& c1, const RGBPixel& c2, int x, int y) {
    Vector3f bary = getBarycentricCoordiantes(p0, p1, p2, x,y);
    return c0*bary.x()+c1*bary.y()+c2*bary.z();
}


//TODO? maybe make this work with clipping when you are bored :)
void triangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, const RGBPixel& c0,const RGBPixel& c1, const RGBPixel& c2) {

	const int x0 = p0.x();
	const int y0 = p0.y();
	
	const int x1 = p1.x();
	const int y1 = p1.y();

	const int x2 = p2.x();
	const int y2 = p2.y();

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
			if (ff0 >= 0 && ff1 >= 0 && ff2 >= 0){
                RGBPixel mixed = mixColor( p0, p1, p2,c0, c1, c2, x, y);
                setPixel(x, y, mixed.x(), mixed.y(), mixed.z());
            }
			ff0 = ff0 + (y0 - y1);
			ff1 = ff1 + (y1 - y2);
			ff2 = ff2 + (y2 - y0);
		}
		f0 = f0 + (x1 - x0);
		f1 = f1 + (x2 - x1);
		f2 = f2 + (x0 - x2);
	}
}



void triangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, GLfloat r, GLfloat g, GLfloat b) {
    triangle(p0, p1, p2, RGBPixel(r,g,b), RGBPixel(r,g,b), RGBPixel(r,g,b));
}

void triangle(const Vector3i& p0, const Vector3i& p1, const Vector3i& p2, GLfloat r, GLfloat g, GLfloat b) {
    triangle(toCartesianCoordinate(p0),toCartesianCoordinate(p1),toCartesianCoordinate(p2),r,g,b);
}
bool compareActiveEdge(const ActiveEdge& a, const ActiveEdge& b) {
	return a.xs < b.xs;
}

bool comparePassiveEdge(const PassiveEdge& a, const PassiveEdge& b) {
    return a.ymin < b.ymin;
}

vector<Vector2i> getClippedPolygon(const vector<Vector2i> &toClip) {

    vector<Vector2i> res = toClip;

    vector<vector<int>> clipper = {
            {0, INT_MIN, INT_MAX, INT_MAX},
            {INT_MIN, INT_MIN, RESOLUTION, INT_MAX},
            {INT_MIN, 0, INT_MAX, INT_MAX},
            {INT_MIN, INT_MIN, INT_MAX, RESOLUTION},
    };

    for (const auto &currentClipper: clipper){
        vector<Vector2i> temp;
        for (int i = 1; i<=res.size(); i++){
            Vector2i p0,p1;
            if(i == res.size()){
                p0 = res[i-1];
                p1 = res[0];
            }
            else{
                p0 = res[i-1];
                p1 = res[i];
            }


            int code1 = Outcode(p0.x(),p0.y(), currentClipper[0], currentClipper[1], currentClipper[2], currentClipper[3]);
            int code2 = Outcode(p1.x(),p1.y(), currentClipper[0], currentClipper[1], currentClipper[2], currentClipper[3]);

            if(code1 != 0 && code2!=0){
                continue;
            }
            else if(code1 == 0 && code2==0){
                temp.emplace_back(p1);
            }
            else if(code1 != 0){

                Vector2i new0, new1;
                getClippedPoints(p0,p1, &new0, &new1,currentClipper[0], currentClipper[1], currentClipper[2], currentClipper[3]);
                temp.emplace_back(new0);
                temp.emplace_back(p1);
            }
            else{
                Vector2i new0, new1;
                getClippedPoints(p0,p1, &new1, &new1,currentClipper[0], currentClipper[1], currentClipper[2], currentClipper[3]);
                temp.emplace_back(new1);
            }
        }
        res = {};
        res = temp;
    }


    return res;
}

void polygon(const vector<Vector2i>& points, GLfloat r, GLfloat g, GLfloat b) {

    vector<Vector2i> clippedPoints = getClippedPolygon(points);
    int minY = 2147483647;
	int maxY = 0;

	vector<PassiveEdge> passive;

	for (int i = 0; i < clippedPoints.size(); i++) {

		PassiveEdge e;
        Vector2i a, be;
		if (i == 0) {
			a = clippedPoints[clippedPoints.size() - 1];
            be = clippedPoints[0];
		}
		else {
			a = clippedPoints[i - 1];
            be = clippedPoints[i];
		}

		e.ymin = a.y() < be.y() ? a.y() : be.y();
		e.xmin = a.y() < be.y() ? a.x() : be.x();
		e.ymax = a.y() >= be.y() ? a.y() : be.y();
		e.xmax = a.y() >= be.y() ? a.x() : be.x();

		passive.push_back(e);
				
	}
    sort(passive.begin(), passive.end(), comparePassiveEdge);

    vector<PassiveEdge> passiveCopy = passive;


	for (Vector2i p : clippedPoints) {
		if (p.y() < minY) {
			minY = p.y();
		}
		if (p.y() > maxY) {
			maxY = p.y();
		}
	}

    vector<ActiveEdge> active;

    for (int y = minY; y <= maxY; y++) {
		// 1) Verschiebe Kanten mit ymin = y von passiver in aktive Liste
        for (auto it = passive.begin();it != passive.end();) {

            if (y == it->ymin && it->ymin != it->ymax) {
                //2. Initialisiere dabei xs = xmin und berechne ∆x
                ActiveEdge toAdd;
                toAdd.ymax = it->ymax;
                toAdd.xs = (float)it->xmin;
                toAdd.dx = (float)(it->xmax - it->xmin) / (float)(it->ymax - it->ymin);
                active.emplace_back(toAdd);
                it = passive.erase(it);
            } else {
                ++it;
            }
        }


		// 3) Entferne Kanten mit ymax = y aus aktiver Liste
        for (auto it = active.begin();it != active.end();) {

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
		for (int i = 0; i <= active.size();i++) {
			if (i % 2 == 0) {
                float endX = active[i + 1].xs;
                int pixelEndX = floor(endX) == endX ? (int) endX-1 : (int)(endX);

				for (int x = ceil(active[i].xs); x <= pixelEndX; x++) {
					setPixel(x, y, r, g, b);
				}
			}
		}


		//6. Inkrementiere y um 1 und alle xs der aktiven Liste um ∆x
		//y++ in for-loop
		for (ActiveEdge& e : active) {
			e.xs += e.dx;
		}

	}

    for(PassiveEdge e : passiveCopy){
        //should use lineMidpoint, but that bugs out so whatevs until i have problems
        lineBresenheim(Vector2i(e.xmin, e.ymin), Vector2i(e.xmax, e.ymax), r,g,b);
    }

}


void polygon(const vector<Vector3i>& points, GLfloat r, GLfloat g, GLfloat b) {
    polygon(toCartesianCoordinates(points),r,g,b);
}

Vector2i getCMSplinePoint(const vector<Vector2f>& pointsf, float t){
    int p0, p1, p2, p3;


    p1 = (int)t+1;
    p2 = p1+1;
    p3 = p2+1;
    p0 = p1-1;
    t = t - (int) t;

    Vector2f res = 0.5 *(  	(2 * pointsf[p1]) +
                             (-pointsf[p0] + pointsf[p2]) * t +
                             (2*pointsf[p0] - 5*pointsf[p1] + 4*pointsf[p2] - pointsf[p3]) * t*t +
                             (-pointsf[p0] + 3*pointsf[p1]- 3*pointsf[p2] + pointsf[p3]) * t*t*t);

    return {res.x(),res.y()};


}

//src https://www.mvps.org/directx/articles/catmull/
//and https://www.youtube.com/watch?v=9_aJGUTePYo
void catmullRom(const vector<Vector2i>& points, int r, int g, int b) {

    if(points.size() < 4){
        cerr << "Catmull Rom Splines need at least 4 points" << endl;
        exit(1);
    }

    vector<Vector2f> pointsf(points.size());
    for (int i = 0; i < points.size(); i++) {
        pointsf[i] = points[i].cast<float>();
    }

    for (float i = 0; i<=pointsf.size()-3; i+= 0.001){
        Vector2i toDraw = getCMSplinePoint(pointsf, i);
        setPixel(toDraw.x(), toDraw.y(), r,g,b);
    }

}






