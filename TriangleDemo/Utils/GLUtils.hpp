//
//  InitWindow.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
// using namespace glm;

#include <iostream>

const GLint WIDTH = 1200, HEIGHT = 800;
const int COLOR_SIZE = 4 * sizeof(float);
const int VERTICE_SIZE = 3 * sizeof(float);
const int TOTAL_SIZE = COLOR_SIZE + VERTICE_SIZE;

// TODO: rename window utils to Utils..
class WindowUtils
{
private:
    
    WindowUtils() {}
    WindowUtils(WindowUtils const&);
    void operator=(WindowUtils const&);
    
public:
        
    static WindowUtils& GetInstance()
    {
        static WindowUtils _instance;
        return _instance;
    }
    

    GLFWwindow * createWindow() {
        glfwInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Project", nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(window);
        
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

        /* Init Glew */
        glewExperimental = GL_TRUE;
        
        if (GLEW_OK != glewInit())
        {
            std::cout << "Failed to initialized GLEW" << std::endl;
            exit(1);
        }

        // Define Viewport
        glViewport(0, 0, screenWidth, screenHeight);
        
        // Options
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS); 
        
        return window;
    }
};
