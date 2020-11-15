//
//  Projection.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 27/10/2020.
//

#pragma once

#include "CGUtils.hpp"

enum PROJECTION_TYPE { ORTHOGRAPHIC, PERSPECTIVE };

const float DEFAULT_NEAR = 0.1;
const float DEFAULT_FAR = 2000.0;
const float ORTHO_SIZE = 10;

class Projection
{
public:
    PROJECTION_TYPE type;
    
    float aspect;
    float near;
    float far;
    
    Projection(PROJECTION_TYPE type, Camera * camera, float near = DEFAULT_NEAR, float far = DEFAULT_FAR) : type(type), camera(camera), near(near), far(far) {
        aspect = (float)CGUtils::GetInstance().SCREEN_WIDTH / ((float)CGUtils::GetInstance().SCREEN_HEIGHT);
    }
    
    mat4 getProjectionMatrix() {
        if (type == PERSPECTIVE) {
            return perspective(camera->fov, aspect, near, far);
        }
        else {
            return ortho<float>(-ORTHO_SIZE * aspect, ORTHO_SIZE * aspect, -ORTHO_SIZE , ORTHO_SIZE, near, far);
        }
    }
    
private:
    Camera * camera;    
};
