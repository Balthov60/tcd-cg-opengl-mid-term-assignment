//
//  Texture.hpp
//  OpenGL MidTermAssignment
//
//  Created by Balthazar Frolin on 06/12/2020.
//

#pragma once
#include <stdio.h>
#include <fstream>

#include "CGUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

class Texture
{

public:
    unsigned int texture;

    Texture(char* filename) {
        load(filename);
    }
    
    void use() {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

private:
    const static string TEXTURE_SOURCE_FOLDER_PATH;

    void load(char* filename) {
        string filePath = TEXTURE_SOURCE_FOLDER_PATH + filename;
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        int width, height, comp;
        unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &comp, 0);

        if(image == nullptr) {
            printf("Unabled to load texture %s\n", filename);
            exit(1);
        }
                    
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);
    }
};

const string Texture::TEXTURE_SOURCE_FOLDER_PATH = "res/textures/";
