//
//  Camera.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 19/10/2020.
//

#pragma once

#include "CGUtils.hpp"

enum CAMERA_DIRECTION { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// Camera values
const GLfloat DEFAULT_YAW = -100.0;
const GLfloat DEFAULT_PITCH = -17;
const vec3 DEFAULT_POSITION = vec3(13.0, 8.8, 30.0);
const vec3 DEFAULT_UP = vec3(0.0, 1.0, 0.0);
const vec3 DEFAULT_FRONT = vec3(0.0, 0.0, -1.0);

// Camera configurations
const GLfloat DEFAULT_SPEED = 40.0;
const GLfloat DEFAULT_SENSITIVTY = 0.10;
const GLfloat DEFAULT_FOV = 45.0;

class Camera
{
public:
    // Used for Projection
    GLfloat fov;
    vec3 position;

    Camera(vec3 position = DEFAULT_POSITION, vec3 world_up = DEFAULT_UP, GLfloat yaw = DEFAULT_YAW, GLfloat pitch = DEFAULT_PITCH)
    : front(DEFAULT_FRONT), position(position), worldUp(world_up), up(world_up), yaw(yaw), pitch(pitch),
      movementSpeed(DEFAULT_SPEED), mouseSensitivity(DEFAULT_SENSITIVTY), fov(DEFAULT_FOV)
    {
        updateVectors();
    }
    
    mat4 getViewMatrix() {
        return lookAt(position, position + front, up);
    }
    
    /**
        Move Camera using translation
     */
    void processTranslationMovement(CAMERA_DIRECTION direction, GLfloat deltaTime)
    {
        GLfloat velocity = movementSpeed * deltaTime;
        
        switch (direction) {
            case FORWARD:
                position += front * velocity;
                break;
                
            case BACKWARD:
                position -= front * velocity;
                break;
                
            case RIGHT:
                position += right * velocity;
                break;
                
            case LEFT:
                position -= right * velocity;
                break;
                
            case UP:
                position += up * velocity;
                break;
                
            case DOWN:
                position -= up * velocity;
        }
        
        // cout << "p " << position.x << " " << position.y << " " << position.z << " " << endl;
    }
    
    /**
        Move Camera using rotation
     */
    void processRotationMovement(GLfloat xOffset, GLfloat yOffset)
    {
        yaw += xOffset * mouseSensitivity;
        pitch += yOffset * mouseSensitivity;
        
        if ( this->pitch > 89.0f )
        {
            this->pitch = 89.0f;
        }
        
        if ( this->pitch < -89.0f )
        {
            this->pitch = -89.0f;
        }
        
        // cout << "p " << this->pitch << " y" << this->yaw << endl;
        
        this->updateVectors();
    }
    
private:
    // Camera Configuration
    vec3 worldUp;

    // Camera Vectors
    vec3 front;
    vec3 right;
    vec3 up;
    // Camera Angles
    GLfloat yaw;
    GLfloat pitch;
    
    // Camera manipulation options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    
    void updateVectors()
    {
        front = normalize(vec3(cos(radians(yaw)) * cos(radians(pitch)),
                               sin(radians(pitch)),
                               sin(radians(yaw)) * cos(radians(pitch))));

        right = normalize(cross(front, worldUp));
        
        up = normalize(cross(right, front));
    }
};

