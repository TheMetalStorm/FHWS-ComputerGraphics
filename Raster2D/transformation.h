//
// Created by arapo on 01.04.2023.
//

#pragma once
#include "simple.h"

Vector3i toHomogeneousCoordinate(const Vector2i& original);

vector<Vector3i> toHomogeneousCoordinates(const vector<Vector2i>& original);

Vector2i toCartesianCoordinate(const Vector3i& original);

vector<Vector2i> toCartesianCoordinates(const vector<Vector3i>& original);

Matrix3f generateTranslationMatrix(float moveX, float moveY);

Matrix3f generateScalingMatrix(float scaleX, float scaleY);

Matrix3f generateRotationMatrix(float byDegree);

Matrix3f generateShearingMatrix(float x, float y);

