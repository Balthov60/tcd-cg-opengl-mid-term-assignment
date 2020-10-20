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


class VertexArray {
    
public:
    
    GLuint array;
    
    /*
        Create VertexArray with a vector of VecterBuffers
        VertexBuffers needs to be ordered according to shader (location) directives
     */
    VertexArray(vector<VertexBuffer> vbos) {
        
        glGenVertexArrays(1, &array);
        
        for (int i = 0; i < vbos.size(); i++) {
            VertexBuffer vbo = vbos[0];
            
            glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
            glBindVertexArray(array);
            
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, vbo.unitSize, GL_FLOAT, GL_FALSE, 0, NULL);
        }
        
        glBindVertexArray(0);
    }

    VertexArray(GLuint verticesQty, VertexBuffer vbo, ShaderProgram program) {
        glGenVertexArrays(1, &array);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
        glBindVertexArray(array);

        // TODO: Should it be here ?
        program.validate();
        
        GLuint positionID = glGetAttribLocation(program.program, "position");
        GLuint colorID = glGetAttribLocation(program.program, "color");

        glEnableVertexAttribArray(positionID);
        glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glEnableVertexAttribArray(colorID);
        glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesQty * VERTICE_SIZE));
        
        glBindVertexArray(0);
    }
    
    void use() {
        glBindVertexArray(array);
    }
};
