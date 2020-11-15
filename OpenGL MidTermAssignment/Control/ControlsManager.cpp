//
//  ControlsManager.cpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 26/10/2020.
//

#include "ControlsManager.hpp"


GLfloat ControlsManager::lastFrame = 0;
GLfloat ControlsManager::deltaTime = 0;

bool ControlsManager::isKeysPressed[];

bool ControlsManager::initMouse = true;
GLfloat ControlsManager::lastX;
GLfloat ControlsManager::lastY;

void ControlsManager::MouseListener(GLFWwindow *window, double x, double y)
{
    if (initMouse) {
        lastX = x;
        lastY = x;
        initMouse = false;
    }
    
    GLfloat xOffset = x - lastX;
    GLfloat yOffset = lastY - y;
    
    lastX = x;
    lastY = y;
    
    CGUtils::GetInstance().camera->processRotationMovement(xOffset, yOffset);
}

void ControlsManager::HandleCameraMovements()
{
    Camera * camera = CGUtils::GetInstance().camera;
    if (ControlsManager::isKeysPressed[GLFW_KEY_W]) {
        camera->processTranslationMovement(FORWARD, deltaTime);
    }
    
    if (ControlsManager::isKeysPressed[GLFW_KEY_S]) {
        camera->processTranslationMovement(BACKWARD, deltaTime);
    }
    
    if (ControlsManager::isKeysPressed[GLFW_KEY_A]) {
        camera->processTranslationMovement(LEFT, deltaTime);
    }
    
    if (ControlsManager::isKeysPressed[GLFW_KEY_D]) {
        camera->processTranslationMovement(RIGHT, deltaTime);
    }
    
    if (ControlsManager::isKeysPressed[GLFW_KEY_LEFT_SHIFT]) {
        camera->processTranslationMovement(DOWN, deltaTime);
    }
    
    if (ControlsManager::isKeysPressed[GLFW_KEY_SPACE]) {
        camera->processTranslationMovement(UP, deltaTime);
    }

    // temp
    if (ControlsManager::isKeysPressed[GLFW_KEY_T]) {
        camera->fov += 0.5f * deltaTime;

    }
    else if (ControlsManager::isKeysPressed[GLFW_KEY_G]) {
        camera->fov -= 0.5f * deltaTime;

    }
    
    if (ControlsManager::isKeysPressed[GLFW_KEY_H]) {
        camera->processRotationMovement(300.0* deltaTime, 0.0);

    }
    else if (ControlsManager::isKeysPressed[GLFW_KEY_F]) {
        camera->processRotationMovement(-300.0* deltaTime, 0.0);
    }
}
