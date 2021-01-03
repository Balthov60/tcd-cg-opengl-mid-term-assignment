//
//  ShaderProgram.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once

#include "CGUtils.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Projection.hpp"
#include "Transformation.hpp"
#include "Lights.hpp"

#define concat(first, second) first second

struct Material {
    vec3 ambiant_light_color = vec3(0.5, 0.5, 0.5);
    vec3 diffuse_light_color = vec3(0.4, 0.4, 0.4);
    vec3 specular_light_color = vec3(1.0, 1.0, 1.0);
    
    float specular_strength = 1.0; // Shininess
    
    Material() {};
    Material(vec3 ambiant_light_color, vec3 diffuse_light_color, vec3 specular_light_color, float specular_strength)
    : ambiant_light_color(ambiant_light_color), diffuse_light_color(diffuse_light_color), specular_light_color(specular_light_color), specular_strength(specular_strength) {};
};

class ShaderProgram
{
public:
    GLuint program;
    Shader vertexShader;
    Shader fragmentShader;
    
    ShaderProgram(Shader vertexShader, Shader fragmentShader)
    : vertexShader(vertexShader), fragmentShader(fragmentShader)
    {
        initProgram();
        compile();
    }
    ShaderProgram(const char* vertexSourceName, const char* fragmentSourceName)
    : vertexShader(Shader(vertexSourceName, GL_VERTEX_SHADER)), fragmentShader(Shader(fragmentSourceName, GL_FRAGMENT_SHADER))
    {
        initProgram();
        compile();
    }
    
    
    void use() {
        glUseProgram(program);
    }
    
    // check if program can execute given the current pipeline state
    void validate() {
        GLint sucess = 0;
        GLchar errorLog[1024];
        
        glValidateProgram(program);
        glGetProgramiv(program, GL_VALIDATE_STATUS, &sucess);
        if (!sucess) {
            glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
            fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
            exit(1);
        }
    }
    
    
    /* *********************** */
    /* SETUP UNIFORM VARIABLES */
    /* *********************** */
    
    // Redefine all of that
    void setupUniformMatrix(Projection projection, Camera * camera, Transformation transform) {
        use();
        linkMatrixUniformVariable(projection.getProjectionMatrix(), "projection");
        linkMatrixUniformVariable(camera->getViewMatrix(), "view");
        
        setupTransformMatrix(transform);
    }
    
    void setupGlobalMatrix(Projection projection, Camera * camera) {
        use();
        linkMatrixUniformVariable(projection.getProjectionMatrix(), "projection");
        linkMatrixUniformVariable(camera->getViewMatrix(), "view");
    }
    void setupGlobalMatrix(Projection projection, Camera * camera, vec3 lightPosition) {
        use();
        linkMatrixUniformVariable(projection.getProjectionMatrix(), "projection");
        linkMatrixUniformVariable(camera->getViewMatrix(), "view");
        
        setupLightUniform(camera, lightPosition);
    }
    
    
    
    void setupTransformMatrix(Transformation transform) {
        use();
        linkMatrixUniformVariable(transform.matrix, "transform");
    }
    void setupMaterialAndTexture(Material material, Texture * texture) {
        linkMaterial(material);
        if (texture == NULL) {
            linkIntegerUniformVariable(0, "texture_present");
        }
        else {
            linkTexture(texture);
        }
    }
    
    void linkMaterial(Material material) {
        linkVectorUniformVariable(material.ambiant_light_color, "material.ambiant_light_color");
        linkVectorUniformVariable(material.diffuse_light_color, "material.diffuse_light_color");
        linkVectorUniformVariable(material.specular_light_color, "material.specular_light_color");
        linkFloatUniformVariable(material.specular_strength, "material.specular_strength");
    }
    void linkTexture(Texture * texture) {
        texture->use();

        GLuint uniformVariableLocation = getUniformVariableLocation("object_texture");
        glUniform1i(uniformVariableLocation, texture->texture);
        
        GLuint texturePresentLocation = getUniformVariableLocation("texture_present");
        glUniform1i(texturePresentLocation, 1);
        
        // glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, texture->texture);
        glUniform1i(uniformVariableLocation, 0 );
    }
    
    /* Light Uniform */
    
    void setupLightUniform(Camera * camera, vec3 lightPosition) {
        linkVectorUniformVariable(camera->position, "view_position");
        linkVectorUniformVariable(lightPosition, "light_position");
    }
    
    void setupAdvancedLightsUniform(Camera * camera, vector<SpotLight> spotLights, vector<PointLight> pointLights, DirectionLight * directionLight = NULL) {
        linkVectorUniformVariable(camera->position, "view_position");
        
        linkDirectionalLight(directionLight);
        
        for (int i = 0; i < spotLights.size(); i++) {
            linkSpotLight(spotLights[i], i);
        }
        linkIntegerUniformVariable((int) spotLights.size(), "SPOT_LIGHT_QTY");
        // printf("load %d spotlights\n", (int) spotLights.size());

        for (int i = 0; i < pointLights.size(); i++) {
            linkPointLight(pointLights[i], i);
        }
        linkIntegerUniformVariable((int) pointLights.size(), "POINT_LIGHT_QTY");
        // printf("load %d pointLight\n", (int) pointLights.size());
    }
    
    void linkDirectionalLight(DirectionLight * directionLight) {
        if (directionLight == NULL) {
            linkIntegerUniformVariable(0, "DIRECTION_LIGHT_PRESENT");
            // printf("load 0 directionalLight\n");
        }
        else {
            linkIntegerUniformVariable(1, "DIRECTION_LIGHT_PRESENT");
            
            linkVectorUniformVariable(directionLight->direction, "directionLight.direction");
            linkVectorUniformVariable(directionLight->ambient, "directionLight.ambient");
            linkVectorUniformVariable(directionLight->diffuse, "directionLight.diffuse");
            linkVectorUniformVariable(directionLight->specular, "directionLight.specular");
            // printf("load 1 directionalLight\n");
        }
    }
    
    void linkPointLight(PointLight pointLight, int index) {
        char variableName[100];
        sprintf(variableName, "pointLights[%d]", index);
        string temp(variableName);
        
        linkVectorUniformVariable(pointLight.position, string(variableName).append(".position").c_str());
        linkVectorUniformVariable(pointLight.ambient, string(variableName).append(".ambient").c_str());
        linkVectorUniformVariable(pointLight.diffuse, string(variableName).append(".diffuse").c_str());
        linkVectorUniformVariable(pointLight.specular, string(variableName).append(".specular").c_str());
        
        linkFloatUniformVariable(pointLight.constant, string(variableName).append(".constant").c_str());
        linkFloatUniformVariable(pointLight.linear, string(variableName).append(".linear").c_str());
        linkFloatUniformVariable(pointLight.quadratic, string(variableName).append(".quadratic").c_str());
    }
    
    void linkSpotLight(SpotLight spotLight, int index) {
        char variableName[100];
        sprintf(variableName, "spotLights[%d]", index);
        
        linkVectorUniformVariable(spotLight.direction, string(variableName).append(".direction").c_str());
        
        linkFloatUniformVariable(spotLight.cutOff, string(variableName).append(".cutOff").c_str());
        linkFloatUniformVariable(spotLight.outerCutOff, string(variableName).append(".outerCutOff").c_str());
        
        // Point Light
        linkVectorUniformVariable(spotLight.pointLight.position, string(variableName).append(".pointLight.position").c_str());
        linkVectorUniformVariable(spotLight.pointLight.ambient, string(variableName).append(".pointLight.ambient").c_str());
        linkVectorUniformVariable(spotLight.pointLight.diffuse, string(variableName).append(".pointLight.diffuse").c_str());
        linkVectorUniformVariable(spotLight.pointLight.specular, string(variableName).append(".pointLight.specular").c_str());
        
        linkFloatUniformVariable(spotLight.pointLight.constant, string(variableName).append(".pointLight.constant").c_str());
        linkFloatUniformVariable(spotLight.pointLight.linear, string(variableName).append(".pointLight.linear").c_str());
        linkFloatUniformVariable(spotLight.pointLight.quadratic, string(variableName).append(".pointLight.quadratic").c_str());
    }
    
    void useFog(bool status) {
        if (status) {
            linkIntegerUniformVariable(1, "useFog");
        }
        else {
            linkIntegerUniformVariable(0, "useFog");
        }
    }
    
private:
    
    void initProgram()
    {
        this->program = glCreateProgram();
        if (program == 0) {
            fprintf(stderr, "Error creating shader program\n");
            exit(1);
        }
    }
    
    void compile()
    {
        GLint sucess = 0;
        GLchar errorLog[1024];
        
        glAttachShader(program, vertexShader.shader);
        glAttachShader(program, fragmentShader.shader);
        
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &sucess);
        if (sucess == 0) {
            glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
            exit(1);
        }
    }
    
    
    /* ****************************** */
    /* GENERAL LINK UNIFORM VARIABLES */
    /* ****************************** */
    
    GLuint getUniformVariableLocation(const char * name)
    {
        GLuint uniformVariableLocation = glGetUniformLocation(program, name);
        if (uniformVariableLocation < 0) {
            fprintf(stderr, "Uniform variable location is negative : '%s'\n", name);
            exit(1);
        }
        return uniformVariableLocation;
    }

    void linkMatrixUniformVariable(const mat4 mat, const char * name)
    {
        GLuint uniformVariableLocation = getUniformVariableLocation(name);
        glUniformMatrix4fv(uniformVariableLocation, 1, GL_FALSE, value_ptr(mat));
    }
    
    void linkVectorUniformVariable(const vec3 vector, const char * name)
    {
        GLuint uniformVariableLocation = getUniformVariableLocation(name);
        glUniform3fv(uniformVariableLocation, 1, value_ptr(vector));
    }
    
    void linkIntegerUniformVariable(const int value, const char * name)
    {
        GLuint uniformVariableLocation = getUniformVariableLocation(name);
        glUniform1i(uniformVariableLocation, value);

    }
    
    void linkFloatUniformVariable(const float value, const char * name)
    {
        GLuint uniformVariableLocation = getUniformVariableLocation(name);
        glUniform1f(uniformVariableLocation, value);

    }
};
