//
// Created by arapo on 01.04.2023.
//

#ifndef FH_CG_TRANSFORMER_H
#define FH_CG_TRANSFORMER_H
#include "transformation.h"

class Transformer {

public:
    explicit Transformer (vector<Vector3i> toTransform);

    Transformer& translate(float byX, float byY);

    Transformer& scale(float byX, float byY);

    Transformer& rotate(float byRadians);
    Transformer &shear(float x, float y);

    vector<Vector3i> transform();

    Transformer &setModelMiddleAsAverageVector();
    Transformer &setModelMiddle(Vector3f localMiddle);


private:
    vector<Vector3f> toTransform ={};
    vector<Matrix3f> allMatrices={};
    Matrix3f localTransform={};

    Vector3f getModelMiddleAsAverageInputVector();

};



#endif //FH_CG_TRANSFORMER_H
