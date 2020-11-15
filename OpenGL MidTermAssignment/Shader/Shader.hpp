//
//  Shader.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once
#include "CGUtils.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <map>

using namespace std;

// TODO: Handle Catalog with maping "ShaderSourceName -> shader" to create it once
class Shader
{
public:
    GLuint shader;
    
    Shader(const char* shaderSourceName, const GLenum shaderType) : shaderSourceName(shaderSourceName), shaderType(shaderType) {
        initShader();
        loadShaderSource();
        compile();
    }

private:
    const static string SHADER_SOURCE_FOLDER_PATH;
    
    const GLenum shaderType;
    const char * shaderSourceName;
    const GLchar * shaderSource;
            
    void initShader() {
        shader = glCreateShader(shaderType);
        if (shader == 0) {
            fprintf(stderr, "Error creating shader : %s\n", shaderSourceName);
            exit(1);
        }
    }
        
    void loadShaderSource() {
        string filePath = SHADER_SOURCE_FOLDER_PATH + shaderSourceName;
        FILE* file;
        file= fopen(filePath.c_str(), "rb");

        if (file == NULL) {
            fprintf(stderr, "Error reading shader %s\n", shaderSourceName);
            exit(1);
        }

        fseek(file, 0L, SEEK_END);
        long size = ftell(file);

        fseek(file, 0L, SEEK_SET);
        char* buf = new char[size + 1];
        fread(buf, 1, size, file);
        buf[size] = '\0';

        fclose(file);

        shaderSource = buf;
        glShaderSource(shader, 1, &shaderSource, NULL);
    }
    
    void compile() {
        GLint success;
        GLchar infoLog[512];
                
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            fprintf(stderr, "Error compiling shader %s\n %s\n", shaderSourceName, infoLog);
            exit(1);
        }
    }
};

const string Shader::SHADER_SOURCE_FOLDER_PATH = "res/shaders/";
