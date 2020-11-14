//
//  Transformation.h
//  TriangleDemo
//
//  Created by Balthazar Frolin on 27/10/2020.
//

#pragma once
#include "CGUtils.hpp"

const mat4 DEFAULT_TRANSFORM = identity<mat4>();

class Transformation
{
public:
    mat4 matrix;
    
    // Keep Scale & Translation data
    vec3 totalScale = vec3(1, 1, 1);
    vec3 totalTranslation = vec3(0, 0, 0);
    
    // Own Axis
    vec3 right = vec3(1, 0, 0);
    vec3 up = vec3(0, 1, 0);
    vec3 front = vec3(0, 0, 1);

    Transformation(mat4 matrix = DEFAULT_TRANSFORM): matrix(matrix) {}
    
    Transformation(vec3 translate, vec3 rotation): matrix(DEFAULT_TRANSFORM) {
        this->translate(translate);
        
        if (rotation.z != 0) {
            this->rotate(rotation.z, vec3(0, 0, 1));
        }
        if (rotation.y != 0) {
            this->rotate(rotation.y, vec3(0, 1, 0));
        }
        if (rotation.x != 0) {
            this->rotate(rotation.x, vec3(1, 0, 0));
        }
    }

    
    void translate(vec3 vector) {
        matrix = glm::translate(matrix, vector);
        totalTranslation += vector;
    }
    
    void scale(float value, vec3 vector = vec3(1, 1, 1)) {
        vec3 scaleVector = value * vector;
        matrix = glm::scale(matrix, scaleVector);
        totalScale *= scaleVector;
    }
    void vectorScale(vec3 scale) {
        matrix = glm::scale(matrix, scale);
    }
    
    void rotate(float degree, vec3 vector) {
        matrix = glm::rotate(matrix, glm::radians(degree), vector);
    }
    void quaternionRotate(quat rotationQuaternion) {
        mat4 rotationMatrix = glm::toMat4(rotationQuaternion);
        matrix = matrix * rotationMatrix;
    }
    
    Transformation operator*(Transformation transform) {
        return Transformation(matrix * transform.matrix);
    }
    
private:
    
};
