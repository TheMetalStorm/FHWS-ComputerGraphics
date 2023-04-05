//
// Created by arapo on 01.04.2023.
//

#include <numeric>
#include "Transformer.h"
#include <Eigen/Core>
#include <algorithm>


Matrix3f multiplyMatrices( std::vector<Matrix3f>& matrices) {
    std::reverse(matrices.begin(), matrices.end());
    // Multiply all matrices in the vector using std::accumulate
    return accumulate(matrices.begin() + 1, matrices.end(), matrices.front(),
                           [](const Matrix3f& result, const Matrix3f& matrix) {
                               return result * matrix;
                           });
}

Transformer::Transformer(vector<Vector3i> toTransform) {

    localTransform.setZero();
    Matrix3f identity {
            {1,0,0},{0,1,0},{0,0,1}
    };
    allMatrices.push_back(identity);

    vector<Vector3f> pointsf(toTransform.size());
    for (int i = 0; i < toTransform.size(); i++) {
        pointsf[i] = toTransform[i].cast<float>();
    }
    this->toTransform={};
    this->toTransform=pointsf;
}

Transformer &Transformer::translate(float byX, float byY) {
    allMatrices.push_back(generateTranslationMatrix(byX, byY));
    return *this;
}

//doestn do anything for circles
Transformer &Transformer::scale(float byX, float byY) {
    allMatrices.push_back(generateScalingMatrix(byX, byY));
    return *this;
}

Transformer &Transformer::rotate(float byRadians) {
    allMatrices.push_back(generateRotationMatrix(byRadians));
    return *this;
}

Transformer &Transformer::shear(float x, float y) {
    allMatrices.push_back(generateShearingMatrix(x,y));
    return *this;
}

vector<Vector3i> Transformer::transform() {
    if(!localTransform.isZero()) {
        allMatrices.push_back(localTransform);
        localTransform.setZero();
    }
    Matrix3f bigMatrix = multiplyMatrices(allMatrices);
    vector<Vector3i> res;

    for (const Vector3f& vec : toTransform){
        Vector3f product = bigMatrix*vec;
        res.emplace_back((int)product.x(), (int)product.y(), (int)product.z());
    }
    return res;
}

Transformer &Transformer::setModelMiddleAsAverageVector() {

    Vector3f localMiddle = getModelMiddleAsAverageInputVector();
    return setModelMiddle(localMiddle);
}

Transformer &Transformer::setModelMiddle(Vector3f localMiddle) {

    if(!localTransform.isZero()) {
        allMatrices.push_back(localTransform);
        localTransform.setZero();
    }

    Matrix3f toOrigin = generateTranslationMatrix(-localMiddle.x(), -localMiddle.y());
    allMatrices.push_back(toOrigin);
    localTransform = toOrigin.inverse();
    return *this;
}

Vector3f Transformer::getModelMiddleAsAverageInputVector() {

    Vector3f res;
    res.setZero();
    Vector3f sum = std::accumulate(toTransform.begin(), toTransform.end(), res);

    return sum / static_cast<float>(toTransform.size());
}
