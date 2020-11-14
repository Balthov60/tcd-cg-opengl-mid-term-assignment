//
//  ControlsManager.h
//  TriangleDemo
//
//  Created by Balthazar Frolin on 26/10/2020.
//
#pragma once
#include "CGUtils.hpp"
#include "Camera.hpp"

class ControlsManager
{
public:
    
    static float deltaTime;

    static bool isKeysPressed[1024];
    
    static GLfloat lastX;
    static GLfloat lastY;
    
    
    static void UpdateDeltaTime()
    {
        GLfloat currentFrame = glfwGetTime();
        if (lastFrame == 0) {
            lastFrame = currentFrame;
        }
        deltaTime = (currentFrame - lastFrame);
        lastFrame = currentFrame;
    }
    
    static void KeyboardListener(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key >= 0 && key < 1024) {
            updateKeysPressed(key, action);
        }
        
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        
    }
    static void MouseListener(GLFWwindow *window, double x, double y);

    // MERGE with keyboard listener ?
    static void HandleCameraMovements();
    
private:
    
    static GLfloat lastFrame;
    static bool initMouse;
    
    static void updateKeysPressed(int key, int action)
    {
        if (action == GLFW_PRESS) {
            isKeysPressed[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            isKeysPressed[key] = false;
        }
    }
};
