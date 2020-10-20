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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// Basic Include
#include <iostream>
#include <string>
using namespace std;


/* Global Constant */

const GLint WIDTH = 1200, HEIGHT = 800;
GLint SCREEN_WIDTH, SCREEN_HEIGHT;

const int COLOR_SIZE = 4 * sizeof(float);
const int VERTICE_SIZE = 3 * sizeof(float);
const int TOTAL_SIZE = COLOR_SIZE + VERTICE_SIZE;


/* Initialisation Class */

class CGUtils
{
private:
    
    CGUtils() {}
    CGUtils(CGUtils const&);
    void operator=(CGUtils const&);
    
public:
        
    GLFWwindow * window;
    
    static CGUtils& GetInstance()
    {
        static CGUtils _instance;
        return _instance;
    }
    
    GLFWwindow * initAndGetWindow() {
        initContext();
        return window;
    }
    
    
private:
    
    /* Initialisation */
    
    void initContext() {
        initGLFW();
        initWindow();
        initGlew();
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        enableOptions();
    }
        
    void initWindow() {
        window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Project", nullptr, nullptr);
        if (window == nullptr)
        {
            cerr << "Failed to create GLFW window" << endl;
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(window);
        
        glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    }
    
    void initGLFW() {
        glfwInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }
    
    void initGlew() {
        glewExperimental = GL_TRUE;
        
        if (GLEW_OK != glewInit())
        {
            cerr << "Failed to initialized GLEW" << endl;
            exit(1);
        }
    }
    
    
    /* OPTIONS */
    
    void enableOptions() {
        enableDepth();
        enableBlend();
    }
    
    // Enable Depth handling
    void enableDepth() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    // Enable Alpha Channel
    void enableBlend() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
};
