#include "CGUtils.hpp"
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ProgramBundle.hpp"

#include <iostream>

using namespace std;

ProgramBundle initEx1()
{
    vec3 vertices[] = { // Based on 0
        // Triangle -1
        vec3(-1.5f, -0.5f, 0.0f), // bottom left
        vec3(-0.5f, -0.5f, 0.0f), // bottom right
        vec3(-1.0f, 0.5f, 0.0f), // top
        // Triangle 0
        vec3(-0.5f, -0.5f, 0.0f), // bottom left
        vec3(0.5f, -0.5f, 0.0f), // bottom right
        vec3(0.0f, 0.5f, 0.0f), // top
        // Triangle 1
        vec3(0.5f, -0.5f, 0.0f), // bottom left
        vec3(1.5f, -0.5f, 0.0f), // bottom right
        vec3(1.0f, 0.5f, 0.0f), // top
    };
    vec4 colors[] = {
        // Triangle -1
        vec4(0.0f, 1.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 1.0f, 1.0f),
        // Triangle 0
        vec4(0.0f, 1.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 1.0f, 1.0f),
        // Triangle 1
        vec4(0.0f, 1.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 1.0f, 1.0f),
    };

    // Set up the shaders
    Shader defaultVertexShader = Shader("transform.vs", GL_VERTEX_SHADER);
    Shader defaultFragmentShader = Shader("default.fs", GL_FRAGMENT_SHADER);

    ShaderProgram program = ShaderProgram(defaultVertexShader, defaultFragmentShader);
    VertexBuffer vb = VertexBuffer(9, vertices, colors);

    // Link the current buffer to the shader
    VertexArray va = VertexArray(9, vb, program);
    
    return ProgramBundle(program, va);
}

ProgramBundle initEx2()
{
    vec3 vertices[] = { // Based on 0
        vec3(1.0f, 0.5f, 0.0f), // top
        vec3(0.5f, -0.5f, 0.0f), // bottom right
        vec3(0.0f, 0.5f, 0.0f) // top
    };
    vec4 colors[] = {
        vec4(0.0f, 1.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 1.0f, 1.0f)
    };
        
    Shader defaultVertexShader = Shader("default.vs", GL_VERTEX_SHADER);
    Shader defaultFragmentShader = Shader("yellow.fs", GL_FRAGMENT_SHADER);
    ShaderProgram program = ShaderProgram(defaultVertexShader, defaultFragmentShader);
    
    VertexBuffer vb = VertexBuffer(3, vertices, colors);
    VertexArray va = VertexArray(3, vb, program);
    
    return ProgramBundle(program, va);
}

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
    
    ProgramBundle pb1 = initEx1();
    ProgramBundle pb2 = initEx2();
    
    // Enabled Alpha Channel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Setup keyboard listener
    glfwSetKeyCallback(window, keyboardListener);
    
    // Display Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 transformMatrix = generateTransformMatrice();
        pb1.use();
        pb1.program.linkMatrixUniformVariable(transformMatrix, "TRANSFORM_MATRIX");
    
        
        pb2.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    
        
        pb1.use();
        glDrawArrays(GL_TRIANGLES, 0, 9);
    

        glfwSwapBuffers(window);
    }
    
    return 0;
}











