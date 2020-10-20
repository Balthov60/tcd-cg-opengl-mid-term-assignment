//
//  VertexBuffer.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once
#include "GLUtils.hpp"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class VertexBuffer {
    
private:
    GLuint verticesQty;
    vec3 * vertices;
    vec4 * colors;
    
public:
    
    GLuint buffer;
    
    VertexBuffer(GLuint verticesQty, vec3 vertices[], vec4 colors[]) {
        this->verticesQty = verticesQty;
        this->vertices = vertices;
        this->colors = colors;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        glBufferData(GL_ARRAY_BUFFER, verticesQty * TOTAL_SIZE, NULL, GL_STATIC_DRAW);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesQty * VERTICE_SIZE, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, verticesQty * VERTICE_SIZE, verticesQty * COLOR_SIZE, colors);
    }
    
    void use() {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }
};
