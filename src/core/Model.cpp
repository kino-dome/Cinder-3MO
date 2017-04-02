//
//  Model.cpp
//  iPhotoWall
//
//  Created by Hesam Ohadi on 15/11/15.
//
//

#include "Model.h"
#include "glm/gtx/matrix_decompose.hpp"

using namespace ci;

Model::Model()
: mModelMatrix(mat4()), mNormalMatrix(mat3()), mModelTranslation(vec3()), mModelOrientation(quat()), mModelScale(vec3(1.0))
{
    
}

Model Model::clone()
{
    Model copy = Model().modelMatrix(mModelMatrix).normalMatrix(mNormalMatrix);
    return copy;
}

void Model::calcModel()
{
    mModelMatrix = mat4(1.0);
    mModelMatrix *= translate(mat4(1.0), mModelTranslation);
    mModelMatrix *= toMat4(mModelOrientation);
    mModelMatrix *= scale(mat4(1.0), mModelScale);
}

void Model::decomposeModel()
{
    vec3 skew;
    vec4 perspective;
    glm::decompose(mModelMatrix, mModelScale, mModelOrientation, mModelTranslation, skew, perspective);
}
