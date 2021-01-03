//
//  VertexBuffer.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once
#include "CGUtils.hpp"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class VertexBuffer {

public:
    
    GLuint buffer;
    size_t unitSize;
    
    template <class T> VertexBuffer(vector<T> &data) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        unitSize = sizeof(T);
        glBufferData(GL_ARRAY_BUFFER, data.size() * unitSize, &(data.at(0)), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    VertexBuffer(GLuint verticesQty , vec3 vertices[], vec4 colors[]) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        glBufferData(GL_ARRAY_BUFFER, verticesQty * TOTAL_SIZE, NULL, GL_STATIC_DRAW);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesQty * VERTICE_SIZE, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, verticesQty * VERTICE_SIZE, verticesQty * COLOR_SIZE, colors);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    VertexBuffer(GLuint verticesQty , vec3 vertices[], vec4 colors[], vec2 texCoords[]) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        glBufferData(GL_ARRAY_BUFFER, verticesQty * TOTAL_SIZE + verticesQty * TEX_COORD_SIZE, NULL, GL_STATIC_DRAW);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesQty * VERTICE_SIZE, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, verticesQty * VERTICE_SIZE, verticesQty * COLOR_SIZE, colors);
        glBufferSubData(GL_ARRAY_BUFFER, verticesQty * VERTICE_SIZE + verticesQty * COLOR_SIZE, verticesQty * TEX_COORD_SIZE, texCoords);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void use() {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }
};
