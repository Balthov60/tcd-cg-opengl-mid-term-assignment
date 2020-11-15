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
#include "Projection.hpp"
#include "Transformation.hpp"
#include "ProgramBundle.hpp"
#include "HierarchicalModel.hpp"
#include "Animation.hpp"

#include "SpiderModelUtils.hpp"

using namespace std;

ProgramBundle createPlan() {
    float planYOffset = 0.0;
    float axisYOffset = planYOffset + 0.1f;
    vec3 vertexArray[] = {
        vec3(-100.0, planYOffset, -100.0), vec3(100.0, planYOffset, -100.0), vec3(-100.0, planYOffset, 100.0),   // Half Plan
        vec3(100.0, planYOffset, 100.0), vec3(-100.0, planYOffset, 100.0), vec3(100.0, planYOffset, -100.0),     // 2HalfPlan
        vec3(-100.0, axisYOffset, 0.0), vec3(100.0, axisYOffset, 0.0), vec3(0.0, axisYOffset, 1.0),                 // XAxis
        vec3(0.0, axisYOffset, -100.0), vec3(0.0, axisYOffset, 100.0), vec3(1.0, axisYOffset, 0.0),                 // ZAxis
        vec3(0.0, -100.0, 0.5), vec3(0.0, 100, 0.5), vec3(1.0, axisYOffset, 0.5),                    // YAxis
        vec3(0.5, -100.0, 0.0), vec3(0.5, 100, 0.0), vec3(0.5, axisYOffset, 1.0),                    // YAxis
    };
    vec4 colorArray[] = {
        vec4(1.0, 1.0, 1.0, 0.25), vec4(1.0, 1.0, 1.0, 0.25), vec4(1.0, 1.0, 1.0, 0.25),       // Plan White transparant
        vec4(1.0, 1.0, 1.0, 0.25), vec4(1.0, 1.0, 1.0, 0.25), vec4(1.0, 1.0, 1.0, 0.25),       // Plan White transparant
        vec4(0.0, 0.0, 1.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0),       // XAxis Blue
        vec4(0.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0),       // ZAxis Green
        vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0),        // YAxis Red
        vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0)        // YAxis Red
    };
    
    ShaderProgram planShader("transform.vs", "default.fs");
    VertexBuffer planBuffer(18, vertexArray, colorArray);
    VertexArray planArray(18, planBuffer, planShader);
    
    return ProgramBundle(planShader, planArray);
}

int main()
{
    // Ini Global Components
    GLFWwindow * window = CGUtils::GetInstance().initAndGetWindow();
    Camera * camera = CGUtils::GetInstance().camera;
    Projection projection = Projection(PERSPECTIVE, camera);
    
    // Ini Main Transforms
    Transformation rootTransform = Transformation();
    Transformation blenderTransform = Transformation();
    blenderTransform.rotate(90, vec3(-1, 0, 0));
        
    // Init Main Graphical Components
    ShaderProgram shaderProgram("simpleVertexShader.vs", "simpleFragmentShader.fs");
    ProgramBundle planProgramBundle = createPlan();
    
    Transformation baseTransform;
    baseTransform.translate(vec3(20, -20, 0));
    baseTransform.rotate(45, vec3(0, 0, 1));
    baseTransform.scale(0.75);
    HierarchicalModel spider = createSpiderModel(baseTransform, 35);
    
    Transformation baseTransform2;
    baseTransform2.translate(vec3(20, 20, 0));
    baseTransform2.rotate(135, vec3(0, 0, 1));
    baseTransform2.scale(0.5);
    HierarchicalModel spider2 = createSpiderModel(baseTransform2, 10);
    
    Transformation baseTransform3;
    baseTransform3.translate(vec3(-20, 20, 0));
    baseTransform3.rotate(-135, vec3(0, 0, 1));
    baseTransform3.scale(0.25);
    HierarchicalModel spider3 = createSpiderModel(baseTransform3, 50);
    
    Transformation baseTransform4;
    baseTransform4.translate(vec3(-20, -20, 0));
    baseTransform4.rotate(-45, vec3(0, 0, 1));
    baseTransform4.scale(0.75);
    HierarchicalModel spider4 = createDemoSpiderModel(baseTransform4, 10);

    // Game loop
    
    while(!glfwWindowShouldClose(window))
    {
        ControlsManager::UpdateDeltaTime();
        
        /* Event */
        glfwPollEvents();
        ControlsManager::HandleCameraMovements();
        
        // Ortho to perspective
        if (ControlsManager::isKeysPressed[GLFW_KEY_O]) {
            projection.type = ORTHOGRAPHIC;
        } else if (ControlsManager::isKeysPressed[GLFW_KEY_P]) {
            projection.type = PERSPECTIVE;
        }
    
        /* Display */
        glClearColor(0.2f, 0.2, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // Link global Matrix
        shaderProgram.setupGlobalMatrix(projection, camera);
        planProgramBundle.program.setupUniformMatrix(projection, camera, rootTransform);
        
        /* Plan & Axis */
        planProgramBundle.use();
        glDrawArrays(GL_TRIANGLES, 0, 18);

        blenderTransform = rootTransform * blenderTransform;
        spider.draw(shaderProgram, blenderTransform);
        spider2.draw(shaderProgram, blenderTransform);
        spider3.draw(shaderProgram, blenderTransform);
        spider4.draw(shaderProgram, blenderTransform);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
