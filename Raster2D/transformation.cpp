//
// Created by arapo on 01.04.2023.
//
#include "transformation.h"
Matrix3f generateRotationMatrix(float byRadians) {
    return Matrix3f{
            {cosf(byRadians), -sinf(byRadians), 0},
            {sinf(byRadians), cosf(byRadians),  0},
            {0,                                 0,                                  1}
    };
}

Matrix3f generateScalingMatrix(float scaleX, float scaleY) {
    return Matrix3f {
            {scaleX,0,0},
            {0,scaleY, 0},
            {0,0,1}
    };
}

Matrix3f generateTranslationMatrix(float moveX, float moveY) {
    return Matrix3f {
            {1,0,moveX},
            {0,1, moveY},
            {0,0,1}
    };
}

Matrix3f generateShearingMatrix(float x, float y) {
    return Matrix3f {
            {1,x,0},
            {0,1, 0},
            {0,0,1}
    }*
            Matrix3f {
                    {1,0,0},
                    {y,1, 0},
                    {0,0,1}
            }
    ;
}

Vector2i toCartesianCoordinate(const Vector3i &original) {

    return {original.x(), original.y()};
}


vector<Vector2i> toCartesianCoordinates(const vector<Vector3i> &original) {
    vector<Vector2i> res;
    res.reserve(original.size());
    transform(original.begin(), original.end(), back_inserter(res), toCartesianCoordinate);
    return res;
}


vector<Vector3i> toHomogeneousCoordinates(const vector<Vector2i> &original) {
    vector<Vector3i> res;
    res.reserve(original.size());
    transform(original.begin(), original.end(), back_inserter(res), toHomogeneousCoordinate);
    return res;
}

Vector3i toHomogeneousCoordinate(const Vector2i &original) {

    return {original.x(), original.y() , 1};
}
