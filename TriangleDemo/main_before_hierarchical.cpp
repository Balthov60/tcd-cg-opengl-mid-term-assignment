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
    // Init Main Components
    GLFWwindow * window = CGUtils::GetInstance().initAndGetWindow();
    Camera * camera = CGUtils::GetInstance().camera;
    Projection projection = Projection(PERSPECTIVE, camera);
    
    //Init Models
    Model spiderBody("spider/spider_body.dae");
    Model spiderUpLeg("spider/spider_leg_up.dae");
    Model spiderDownLeg("spider/spider_leg_down.dae");
    
    // Init Programs
    ProgramBundle planProgramBundle = createPlan();

    ShaderProgram shaderProgram("simpleVertexShader.vs", "simpleFragmentShader.fs");
    ShaderProgram shaderProgram2("simpleVertexShader.vs", "simpleFragmentShader.fs");
    glUseProgram(NULL);
            
    // Game loop
    
    while(!glfwWindowShouldClose(window))
    {
        /* Idle */
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
        
        // Root of the Hierarchy
        Transformation rootTransform = Transformation();
        
        /* Plan & Axis */
        planProgramBundle.use();
        planProgramBundle.program.setupUniformMatrix(projection, camera, rootTransform);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        
        /* Spider Body */
        rootTransform.translate(vec3(0, 4, 0));
        
        shaderProgram.use();
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        spiderBody.draw(shaderProgram, rootTransform);
        
        /* Spider Legs */
        /* Blender Transform */
        Transformation blenderTransform = Transformation();
        blenderTransform.rotate(90, vec3(-1, 0, 0));
        
        // Right Leg 1
        // Top
        Transformation rightLegTopTransform = Transformation();
        if (ControlsManager::isKeysPressed[GLFW_KEY_U]) {
            rightLegTopTransform.translate(vec3(2.41201, 2.47272, 1.86086));
            rightLegTopTransform.rotate(-18.2539, vec3(0, 0, 1));
            rightLegTopTransform.rotate(-20.5888, vec3(0, 1, 0));
            rightLegTopTransform.rotate(96.616, vec3(1, 0, 0));
        }
        else {
            rightLegTopTransform.translate(vec3(1.71192, 2.47272, 1.38035));
            rightLegTopTransform.rotate(-17.1, vec3(0, 0, 1));
            rightLegTopTransform.rotate(90, vec3(1, 0, 0));
        }
        
        Transformation rightLegTopTransformSave = rightLegTopTransform;
        rightLegTopTransform = blenderTransform * rightLegTopTransform;
        rightLegTopTransform = rootTransform * rightLegTopTransform;
        
        shaderProgram.use();
        shaderProgram.setupUniformMatrix(projection, camera, rightLegTopTransform);
        spiderUpLeg.draw(shaderProgram, rightLegTopTransform);
        
        
        // Bottom
        Transformation rightLegBottomTransform = Transformation();
        rightLegBottomTransform.translate(vec3(8.69604, 2.24185, 0.360813));
        rightLegBottomTransform = rightLegTopTransformSave * rightLegBottomTransform;
        rightLegBottomTransform = blenderTransform * rightLegBottomTransform;
        rightLegBottomTransform = rootTransform * rightLegBottomTransform;

        shaderProgram.use();
        shaderProgram.setupUniformMatrix(projection, camera, rightLegBottomTransform);
        spiderDownLeg.draw(shaderProgram, rightLegBottomTransform);
        
        // Right Leg 2
        // Top
        Transformation rightLegTopTransform_2 = Transformation();
        rightLegTopTransform_2.translate(vec3(1.46798, 3.95067, 1.38035));
        rightLegTopTransform_2.rotate(0.09592, vec3(0, 0, 1));
        rightLegTopTransform_2.rotate(90, vec3(1, 0, 0));
        rightLegTopTransform_2 = blenderTransform * rightLegTopTransform_2;
        rightLegTopTransform_2 = rootTransform * rightLegTopTransform_2;
        
        shaderProgram.use();
        shaderProgram.setupGlobalMatrix(projection, camera);
        spiderUpLeg.draw(shaderProgram, rightLegTopTransform_2);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
