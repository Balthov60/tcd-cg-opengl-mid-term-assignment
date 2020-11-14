//
//  CGUtils.cpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 26/10/2020.
//

#include "CGUtils.hpp"

void CGUtils::initInputListeners() {
    glfwSetKeyCallback(window, ControlsManager::KeyboardListener);
    if (ENABLE_MOUSE) {
        glfwSetCursorPosCallback(window, ControlsManager::MouseListener);
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
