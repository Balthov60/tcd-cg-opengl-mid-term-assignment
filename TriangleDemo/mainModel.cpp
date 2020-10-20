#include "GLUtils.hpp"
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ProgramBundle.hpp"
#include "Model.hpp"
#include "Camera.hpp"

#include <iostream>

using namespace std;
using namespace glm;

float rotateValue = 0.0f;
float translateValue = 0.0f;
void keyboardListener(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        rotateValue += 0.1f;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        rotateValue -= 0.1f;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        translateValue -= 0.1f;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        translateValue += 0.1f;
    }
}

mat4 generateTransformMatrice() {

    mat4 translationMatrix = mat4(1.0f);
    mat4 rotationMatrix = mat4(1.0f);
    
    translationMatrix = translate(mat4(1.0f), vec3(translateValue, 0.0f, 0.0f));
    rotationMatrix = rotate(mat4(1.0f), rotateValue , vec3(0.0f, 1.0f, 0.0f));
    
    return translationMatrix * rotationMatrix;
}

int main(int argc, char** argv){
    GLFWwindow *window = WindowUtils::GetInstance().createWindow();

    ShaderProgram program = ShaderProgram("model_loading.vs", "model_loading.fs");
    Model model = Model("res/models/nanosuit.blend");
    
    // Enabled Alpha Channel
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Setup keyboard listener
    // glfwSetKeyCallback(window, keyboardListener);
    
    // Camera ??
    Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
    
    // Projection ??
    mat4 projection = perspective(camera.GetZoom(), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
    
    // Display Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        program.use();
        
        mat4 view = camera.GetViewMatrix();
        program.linkMatrixUniformVariable(value_ptr(projection), "projection");
        program.linkMatrixUniformVariable(value_ptr(view), "view");
        
        // Draw the loaded model
        mat4 transform;
        transform = translate(transform, vec3(0.0f, -1.75f, 0.0f));
        transform = scale(transform, vec3(0.2f, 0.2f, 0.2f));
        program.linkMatrixUniformVariable(value_ptr(transform), "transform");
                
        model.draw(program);
    
        glfwSwapBuffers(window);
        printf("here");
    }
    
    return 0;
}











