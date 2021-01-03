//
//  VertexArray.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once
#include "CGUtils.hpp"
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"

#include <vector>

class VertexArray {
    
public:
    
    GLuint array;
    
    /*
        Create VertexArray with a vector of VecterBuffers
        VertexBuffers needs to be ordered according to shader (location) directives
     */
    VertexArray(vector<VertexBuffer> &vbos) {
        glGenVertexArrays(1, &array);
        
        for (int i = 0; i < vbos.size(); i++) {
            vbos[i].use();
            glBindVertexArray(array);
            
            glEnableVertexAttribArray(i);
            int size = 3;
            if (i == 2) size = 2;
            glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, 0, NULL);
            glBindVertexArray(0);
        }
    }

    VertexArray(GLuint verticesQty, VertexBuffer vbo, ShaderProgram program) {
        glGenVertexArrays(1, &array);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
        glBindVertexArray(array);
        
        GLuint positionID = glGetAttribLocation(program.program, "position");
        GLuint colorID = glGetAttribLocation(program.program, "color");
        GLuint texId = glGetAttribLocation(program.program, "texCoords");

        glEnableVertexAttribArray(positionID);
        glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glEnableVertexAttribArray(colorID);
        glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesQty * VERTICE_SIZE));
        
        glEnableVertexAttribArray(texId);
        glVertexAttribPointer(texId, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesQty * VERTICE_SIZE + verticesQty * COLOR_SIZE));
        
        glBindVertexArray(0);
    }
    
    void use() {
        glBindVertexArray(array);
    }
};
