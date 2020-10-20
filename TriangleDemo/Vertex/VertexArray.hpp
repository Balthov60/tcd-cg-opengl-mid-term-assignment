//
//  VertexArray.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once
#include "GLUtils.hpp"
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"


class VertexArray {
    
public:
    
    GLuint array;
    
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
    }
    
    void use() {
        glBindVertexArray(0);
        glBindVertexArray(array);
    }
};
