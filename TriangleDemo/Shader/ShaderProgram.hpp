//
//  ShaderProgram.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once

#include "CGUtils.hpp"
#include "Shader.hpp"

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
    
    
    void use()
    {
        glUseProgram(program);
    }
    
    /*
     Add matrix uniform variable
     Program need to be loaded through use() before calling this method.
     */
    void linkMatrixUniformVariable(const mat4 mat, const char * name)
    {
        GLuint uniformVariableLocation = getUniformVariableLocation(name);
        glUniformMatrix4fv(uniformVariableLocation, 1, GL_FALSE, value_ptr(mat));
    }
    
    // check if program can execute given the current pipeline state
    void validate()
    {
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
    
    GLuint getUniformVariableLocation(const char * name)
    {
        GLuint uniformVariableLocation = glGetUniformLocation(program, name);
        if (uniformVariableLocation < 0) {
            fprintf(stderr, "Uniform variable location is negative : '%s'\n", name);
            exit(1);
        }
        return uniformVariableLocation;
    }
};
