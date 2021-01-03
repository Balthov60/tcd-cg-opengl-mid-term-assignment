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
#include "Model.hpp"
#include "Projection.hpp"
#include "Transformation.hpp"
#include "HierarchicalModel.hpp"
#include "Animation.hpp"
#include "ProgramBundle.hpp"
#include "BoidManager.hpp"

#include "SpiderModelUtils.hpp"
#include "TunelModelUtils.hpp"

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
    
    vec2 textureArray[] = {
        vec2(0, 0), vec2(0, 1), vec2(1, 0),
        vec2(1, 1), vec2(1, 0), vec2(0, 1),
        vec2(0, 0), vec2(1, 1), vec2(1, 1),
        vec2(0, 0), vec2(1, 1), vec2(1, 1),
        vec2(0, 0), vec2(1, 1), vec2(1, 1),
        vec2(0, 0), vec2(1, 1), vec2(1, 1)
    };
    
    ShaderProgram planShader("transform.vs", "default.fs");
    VertexBuffer planBuffer(18, vertexArray, colorArray, textureArray);
    VertexArray planArray(18, planBuffer, planShader);
    
    return ProgramBundle(planShader, planArray);
}

#define LIGHT_SPEED 15
vec3 updateLightPosition(vec3 lightPosition) {
    if (ControlsManager::isKeysPressed[GLFW_KEY_LEFT]) {
        lightPosition.x += LIGHT_SPEED * ControlsManager::deltaTime;
    } else if (ControlsManager::isKeysPressed[GLFW_KEY_RIGHT]) {
        lightPosition.x -= LIGHT_SPEED * ControlsManager::deltaTime;
    }
    if (ControlsManager::isKeysPressed[GLFW_KEY_ENTER]) {
        lightPosition.y += LIGHT_SPEED * ControlsManager::deltaTime;
    } else if (ControlsManager::isKeysPressed[GLFW_KEY_RIGHT_SHIFT]) {
        lightPosition.y -= LIGHT_SPEED * ControlsManager::deltaTime;
    }
    if (ControlsManager::isKeysPressed[GLFW_KEY_UP]) {
        lightPosition.z += LIGHT_SPEED * ControlsManager::deltaTime;
    } else if (ControlsManager::isKeysPressed[GLFW_KEY_DOWN]) {
        lightPosition.z -= LIGHT_SPEED * ControlsManager::deltaTime;
    }
    
    return lightPosition;
}

int mode = 1;
int light = 3;

vec3 lightPosition1 = vec3(85.0, 20.0, 85.0);
vec3 lightPosition2 = vec3(85.0, 20.0, 85.0);

void updateModeAndLight() {
    if (ControlsManager::isKeysPressed[GLFW_KEY_K]) { // No Light
        mode = 1;
    }
    else if (ControlsManager::isKeysPressed[GLFW_KEY_L]) { // Display Boids
        if (mode == 1 || mode == 3) {
            BoidManager boidManager = BoidManager();
        }
        mode = 2;
    }
    else if (ControlsManager::isKeysPressed[GLFW_KEY_J]) { // All Light
        mode = 3;
    }
    else if (ControlsManager::isKeysPressed[GLFW_KEY_F]) { // Fog
        mode = 4;
    }
    
    if (ControlsManager::keyPressedFirstTime(GLFW_KEY_B)) { // One / Multiple Light
        if (light == 2) {
            light = 4;
        }
        else if (light == 4) {
            light = 2;
        }
        else if (light == 1) {
            light = 3;
        }
        else if (light == 3) {
            light = 1;
        }
    } else if (ControlsManager::keyPressedFirstTime(GLFW_KEY_N)) { // Switch light control
        if (light == 2) {
            light = 1;
        }
        else if (light == 1) {
            light = 2;
        }
        else if (light == 3) {
            light = 4;
        }
        else if (light == 4) {
            light = 3;
        }
    }
    
    if (light == 1 || light == 3) {
        lightPosition1 = updateLightPosition(lightPosition1);
    }
    if (light == 2 || light == 4) {
        lightPosition2 = updateLightPosition(lightPosition2);
    }
}

int main()
{
    /* Global Init */
    // Main Components
    GLFWwindow * window = CGUtils::GetInstance().initAndGetWindow();
    Camera * camera = CGUtils::GetInstance().camera;
    Projection projection = Projection(PERSPECTIVE, camera);
    
    // Init Main Transforms
    Transformation rootTransform = Transformation();
    Transformation blenderTransform = Transformation();
    blenderTransform.rotate(90, vec3(-1, 0, 0));
        
    // Init Main Graphical Components
    ShaderProgram shaderProgram("illuminationVertexShader.vs", "advancedIlluminationFragmentShader.fs");
    ProgramBundle planProgramBundle = createPlan();
    DirectionLight mainDirectionLight(vec3(0, -1, 0));
    
    /* Init Animated Spiders */
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

    /* Init Lights */
    
    Transformation lightTransform1;
    lightTransform1.translate(lightPosition1);

    Transformation lightTransform2;
    lightTransform1.translate(lightPosition2);
    
    Model * lightModel = new Model("cube.dae");
    lightModel->material = Material(vec3(0.24725, 0.1995, 0.0745), vec3(0.75164, 0.60648, 0.22648), vec3(0.628281, 0.555802, 0.366065), 0.4);
    HierarchicalModel light1(lightModel, lightTransform1);
    HierarchicalModel light2(lightModel, lightTransform2);

    /* Init Texture Demo Items */
    Texture * texture1 = new Texture("brique.jpg");

    Material emerald(vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633), 0.6);
    Material blackRubber(vec3(0.02, 0.02, 0.02), vec3(0.01, 0.01, 0.01), vec3(0.4, 0.4, 0.4), 0.078125);
    Material bronze(vec3(0.19125, 0.0735, 0.0225), vec3(0.7038, 0.27048, 0.0828), vec3(0.256777, 0.137622, 0.086014), 0.1);
    Material greenPlastic(vec3(0.0, 0.0, 0.0), vec3(0.1, 0.35, 0.1), vec3(0.45, 0.55, 0.45), 0.25);
    Material brick(vec3(0.05, 0.0, 0.0), vec3(0.58, 0.49, 0.46), vec3(0.7, 0.04, 0.04), 0.078125);
    
    Model * block1 = new Model("cube.dae");
    block1->material = brick;
    block1->texture = texture1;
    
    Model * block2 = new Model("cube.dae");
    block2->material = emerald;
    Model * block3 = new Model("cube.dae");
    block3->material = blackRubber;
    Model * block4 = new Model("cube.dae");
    block4->material = bronze;
    Model * block5 = new Model("cube.dae");
    block5->material = greenPlastic;
    
    /* Init Crowd Models */
    Transformation boidTransform;
    boidTransform.translate(vec3(77.925, -185, 0.2));
    
    Model * _avoidModel = new Model("cube.dae");
    _avoidModel->material = Material(vec3(0.19125, 0.0735, 0.0225), vec3(0.7038, 0.27048, 0.0828), vec3(0.256777, 0.137622, 0.086014), 0.1);
    HierarchicalModel avoidModel(_avoidModel, boidTransform);
    
    HierarchicalModel boidSpider = createSpiderModel(Transformation(), 50);
    boidSpider.animation = NULL;
    boidSpider.baseTransform = boidTransform;
    
    BoidManager boidManager;

    HierarchicalModel tunel = createTunelModel(Transformation(vec3(0, 0, 0), vec3(0, 0, 0)));
    Transformation tunnelRootTransform = Transformation(vec3(100, 0, 0), vec3(0, 0, 0));
    tunnelRootTransform.scale(2);
    
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
        
        // Update Light for demo
        updateModeAndLight();
        light1.baseTransform = Transformation(lightPosition1, vec3(0, 0, 0));
        light2.baseTransform = Transformation(lightPosition2, vec3(0, 0, 0));

        /* Display */
        vec4 cleanColor = vec4(0.05, 0.05, 0.05, 1.0f);
        if (mode == 4) {
            cleanColor = vec4(0.85, 0.85, 0.85, 1.0);
        }

        glClearColor(cleanColor.r, cleanColor.g, cleanColor.b, cleanColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        // Link global Matrix
        shaderProgram.setupGlobalMatrix(projection, camera);
        
        /* Fog */
        if (mode == 4) {
            shaderProgram.useFog(true);
        }
        else {
            shaderProgram.useFog(false);
        }
        
        /* Lights */
        vector<SpotLight> spotLights;
        vector<PointLight> pointLights;
        DirectionLight * directionLight = NULL;
        
        if (mode == 1) {
            if (light == 1) {
                pointLights.push_back(PointLight(lightPosition1));
            }
            else if (light == 2) {
                pointLights.push_back(PointLight(lightPosition2));
            }
            else if (light > 2) {
                pointLights.push_back(PointLight(lightPosition1));
                pointLights.push_back(PointLight(lightPosition2));
            }
        }
        else if (mode == 2 || mode == 4) {
            directionLight = &mainDirectionLight;
        }
        if (mode == 3) {
            spotLights.push_back(SpotLight(camera->position, camera->front));
        }
        
        shaderProgram.setupAdvancedLightsUniform(camera, spotLights, pointLights, directionLight);
        
        if (mode == 1) {
            if (light == 1) {
                light1.draw(shaderProgram, rootTransform);
            }
            else if (light == 2) {
                light2.draw(shaderProgram, rootTransform);
            }
            else if (light > 2) {
                light1.draw(shaderProgram, rootTransform);
                light2.draw(shaderProgram, rootTransform);
            }
        }
        
        /* Plan & Axis */
        if (mode == 2 || mode == 1) {
            planProgramBundle.program.setupUniformMatrix(projection, camera, rootTransform);
            planProgramBundle.use();
            glDrawArrays(GL_TRIANGLES, 0, 18);
        }
            
        /* Crowd */
        if (mode == 2) {
            boidManager.update();
            
            vector<Transformation> boidsTransforms = boidManager.getBoidsTransforms(boidTransform);
            for (Transformation &transform : boidsTransforms) {
                boidSpider.baseTransform = transform;
                boidSpider.draw(shaderProgram, blenderTransform);
            }
            
            vector<Transformation> avoidsTransforms = boidManager.getAvoidsTransforms(boidTransform);
            for (Transformation &transform : avoidsTransforms) {
                avoidModel.baseTransform = transform;
                avoidModel.draw(shaderProgram, blenderTransform);
            }
        }
        
        blenderTransform = rootTransform * blenderTransform;
        spider.draw(shaderProgram, blenderTransform);
        spider2.draw(shaderProgram, blenderTransform);
        spider3.draw(shaderProgram, blenderTransform);
        spider4.draw(shaderProgram, blenderTransform);
        
        block1->draw(shaderProgram, Transformation(vec3(75, 1, 75), vec3(0, 0, 0)));
        block2->draw(shaderProgram, Transformation(vec3(80, 1, 75), vec3(0, 0, 0)));
        block3->draw(shaderProgram, Transformation(vec3(85, 1, 75), vec3(0, 0, 0)));
        block4->draw(shaderProgram, Transformation(vec3(90, 1, 75), vec3(0, 0, 0)));
        block5->draw(shaderProgram, Transformation(vec3(95, 1, 75), vec3(0, 0, 0)));
        
        tunel.draw(shaderProgram, tunnelRootTransform);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
