//
// Created by arapo on 01.04.2023.
//

#ifndef FH_CG_TRANSFORMATION_H
#define FH_CG_TRANSFORMATION_H
#include "simple.h"

Vector3i toHomogeneousCoordinate(const Vector2i& original){

    return {original.x(), original.y() , 1};
}

vector<Vector3i> toHomogeneousCoordinates(const vector<Vector2i>& original){
    vector<Vector3i> res;
    res.reserve(original.size());
    transform(original.begin(), original.end(), back_inserter(res), toHomogeneousCoordinate);
    return res;
}

#endif //FH_CG_TRANSFORMATION_H
