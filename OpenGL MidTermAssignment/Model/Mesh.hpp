//
//  Mesh.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 19/10/2020.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>

#include "CGUtils.hpp"
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"

typedef struct
{
    GLsizei verticesCount;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> textureCoords;
} MeshData;

class Mesh
{
    
public:
    Mesh(MeshData data) : data(data) {
        vertexBuffers.push_back(VertexBuffer(data.vertices));
        vertexBuffers.push_back(VertexBuffer(data.normals));
        
        vertexArray = new VertexArray(vertexBuffers);
    }
    
    
    void draw(ShaderProgram shaderProgram)
    {
        vertexArray->use();
        shaderProgram.use();
        shaderProgram.validate();
        
        glDrawArrays(GL_TRIANGLES, 0, data.verticesCount);
    }
    
    ~Mesh() {
       delete vertexArray;
    }
    
private:

    MeshData data;
    vector<VertexBuffer> vertexBuffers;
    VertexArray * vertexArray;
};
