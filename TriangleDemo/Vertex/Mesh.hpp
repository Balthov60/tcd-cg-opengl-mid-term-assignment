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

using namespace std;

struct Vertex
{
    vec3 position;
    vec3 normal;
    
    // ??
    vec2 texCoords;
};

struct Texture
{
    GLuint id;
    string type;
    aiString path;
};

typedef struct
{
    GLsizei verticesCount;
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> textureCoords;
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
        shaderProgram.use();
        vertexArray->use();
        
        shaderProgram.validate();
        glDrawArrays(GL_TRIANGLES, 0, data.verticesCount);
    }
    
    ~Mesh() {
        printf("Delete Mesh");
        delete vertexArray;
    }
    
private:

    MeshData data;
    vector<VertexBuffer> vertexBuffers;
    VertexArray * vertexArray;
};
