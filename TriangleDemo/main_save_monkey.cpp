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
    GLFWwindow * window = CGUtils::GetInstance().initAndGetWindow();
    Camera * camera = CGUtils::GetInstance().camera;
    Projection projection = Projection(PERSPECTIVE, camera);
    
    // Prepare Data
    ShaderProgram shaderProgram("simpleVertexShader.vs", "simpleFragmentShader.fs");
    Model model("monkey/monkeyhead_smooth.dae");
    
    
    // Prepare Plan
    ProgramBundle planProgramBundle = createPlan();
                     
            
    // Game loop
    GLfloat rotate_y = 0.0f;
    while(!glfwWindowShouldClose(window))
    {
        /* Idle */
        ControlsManager::UpdateDeltaTime();
        
        // Rotate the model slowly around the y axis at 20 degrees per second
        rotate_y += 20.0f * ControlsManager::deltaTime;
        rotate_y = fmodf(rotate_y, 360.0);
        rotate_y = 0;
        // Translate desactivated
        float translateValue = 0;

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
        glClearColor(0.2f, radians(rotate_y / 10), 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // Root of the Hierarchy
        Transformation rootTransform = Transformation();
        
        /* Plan & Axis */
        planProgramBundle.use();
        planProgramBundle.program.setupUniformMatrix(projection, camera, rootTransform);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        /* Models */
        shaderProgram.use();

        //rootTransform.translate(vec3(translateValue, 0, 0));
        //rootTransform.rotate(rotate_y, vec3(0, 0, 1));
        //rootTransform.rotate(90, vec3(1, 0, 0));
        //rootTransform.translate(vec3(0, 0.0, -10.0));

        
        // Test Scale after a Translation
        rootTransform.rotate(90, vec3(0, 0, 0));

        rootTransform.ownTranslate(vec3(10.0, 0.0, 0.0));
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        model.draw(shaderProgram);
        
        rootTransform.ownTranslate(vec3(-10.0, 0.0, 0.0));
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        model.draw(shaderProgram);
        
        rootTransform.ownTranslate(vec3(-10, 0.0, 0.0));
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        model.draw(shaderProgram);
        

        /*
        // Test Translate after a scale - translate altered - test with no ScaleTranslate
        rootTransform.scale(2);
        
        rootTransform.noScaleTranslate(vec3(0, -10.0, 0));
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        model.draw(shaderProgram);
        
        rootTransform.noScaleTranslate(vec3(0, 10.0, 0));
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        model.draw(shaderProgram);
        
        rootTransform.noScaleTranslate(vec3(0, 10.0, 0));
        shaderProgram.setupUniformMatrix(projection, camera, rootTransform);
        model.draw(shaderProgram);
         
        // Reference
        Transformation referenceTransformation = Transformation();
        referenceTransformation.scale(2);
        referenceTransformation.noScaleTranslate(vec3(0, 0.0, 100.0));
        shaderProgram.setupUniformMatrix(projection, camera, referenceTransformation);
        model.draw(shaderProgram);
        
        referenceTransformation = Transformation();
        referenceTransformation.scale(2);
        referenceTransformation.noScaleTranslate(vec3(0, -10.0, 10.0));
        shaderProgram.setupUniformMatrix(projection, camera, referenceTransformation);
        model.draw(shaderProgram);
        
        referenceTransformation = Transformation();
        referenceTransformation.scale(2);
        referenceTransformation.noScaleTranslate(vec3(0, 10.0, 10.0));
        shaderProgram.setupUniformMatrix(projection, camera, referenceTransformation);
        model.draw(shaderProgram);
        */
        
        // Reference
        
        Transformation referenceTransformation = Transformation();
        referenceTransformation.translate(vec3(10, 0, -10));
        referenceTransformation.rotate(90, vec3(0, 0, 0));
        shaderProgram.setupUniformMatrix(projection, camera, referenceTransformation);
        model.draw(shaderProgram);
        
        referenceTransformation = Transformation();
        referenceTransformation.translate(vec3(0, 0, -10));
        referenceTransformation.rotate(90, vec3(0, 0, 0));
        shaderProgram.setupUniformMatrix(projection, camera, referenceTransformation);
        model.draw(shaderProgram);
        
        referenceTransformation = Transformation();
        referenceTransformation.translate(vec3(-10, 0, -10));
        referenceTransformation.rotate(90, vec3(0, 0, 0));
        shaderProgram.setupUniformMatrix(projection, camera, referenceTransformation);
        model.draw(shaderProgram);


        
        /*
        // update uniforms
        Transformation secondTransform = Transformation();
        secondTransform.translate(vec3(10, 0, 0));
        secondTransform.rotate(45, vec3(0, 0, -1));
        // secondTransform.translate(vec3(20000, 0, 0));
        // secondTransform.scale(4.0);
        // secondTransform.translate(vec3((-19995 / 4), 0, 0));

        
        
        shaderProgram.use();
        shaderProgram.setupUniformMatrix(projection, camera, secondTransform);
        
        model.draw(shaderProgram);


        /*
        // Set up the child matrix
        Transformation childTransform;
        childTransform.rotate(180.0f, vec3(0, 0, 1));
        childTransform.rotate(rotate_y, vec3(0, 1, 0));
        childTransform.translate(vec3(0.0f, -1.9f, 0.0f));

        // Apply the root matrix to the child matrix
        childTransform = rootTransform * childTransform;

        // Update the appropriate uniform and draw the mesh again
        shaderProgram.use();
        shaderProgram.linkMatrixUniformVariable(childTransform.matrix, "transform");

        model.draw(shaderProgram);
         */
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
