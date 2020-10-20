// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>

// GL includes

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "CGUtils.hpp"
#include "ShaderProgram.hpp"
#include "Model.hpp"

using namespace std;

GLfloat rotate_y = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main( )
{
    
    GLFWwindow * window = CGUtils::GetInstance().initAndGetWindow();
    
    //init function
    ShaderProgram shaderProgram("simpleVertexShader.txt", "simpleFragmentShader.txt");
    Model model = Model("monkey/monkeyhead_smooth.dae");

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        /* Idle */
        GLfloat currentFrame = glfwGetTime();
        if (lastFrame == 0) {
            lastFrame = currentFrame;
        }
        float delta = (currentFrame - lastFrame);
        lastFrame = currentFrame;

        // Rotate the model slowly around the y axis at 20 degrees per second
        rotate_y += 20.0f * delta;
        rotate_y = fmodf(rotate_y, 360.0f);
        
        
        /* Event */
        glfwPollEvents();
        
        /* Display */
        glClearColor(0.2f, radians(rotate_y / 10), 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        // Root of the Hierarchy
        mat4 view = identity<mat4>();
        mat4 persp_proj = perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        mat4 modelMatrice = identity<mat4>();
        modelMatrice = rotate(modelMatrice, radians(rotate_y), vec3(0, 0, 1));
        view = translate(view, vec3(0.0, 0.0, -10.0f));

        // update uniforms
        shaderProgram.linkMatrixUniformVariable(persp_proj, "proj");
        shaderProgram.linkMatrixUniformVariable(view, "view");
        shaderProgram.linkMatrixUniformVariable(modelMatrice, "model");

        model.draw(shaderProgram);

        
        // Set up the child matrix
        mat4 modelChild = identity<mat4>();
        modelChild = rotate(modelChild, radians(180.0f), vec3(0, 0, 1));
        modelChild = rotate(modelChild, radians(rotate_y), vec3(0, 1, 0));
        modelChild = translate(modelChild, vec3(0.0f, -1.9f, 0.0f));

        // Apply the root matrix to the child matrix
        modelChild = modelMatrice * modelChild;

        // Update the appropriate uniform and draw the mesh again
        shaderProgram.linkMatrixUniformVariable(modelChild, "model");

        model.draw(shaderProgram);

        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
