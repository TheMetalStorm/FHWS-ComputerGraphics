#pragma once
#include "Vector.h"

class Camera
{
public:
	Vector eye;
	Vector lookAt;
	Vector up;

public:
	Camera(Vector _eye, Vector _lookAt, Vector _up) : eye(_eye), lookAt(_lookAt), up(_up) {};
	Vector getForward() {
		return lookAt.vsub(eye).normalize();
	};

	Vector getRight() {
		return getForward().cross(up).normalize();
	}

	Vector getActualUp() {
		return getRight().cross(getForward()).normalize();
	}
};