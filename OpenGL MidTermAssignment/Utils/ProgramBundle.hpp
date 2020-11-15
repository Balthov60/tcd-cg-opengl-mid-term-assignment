//
//  ProgramBundle.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 08/10/2020.
//

#pragma once
#include "ShaderProgram.hpp"
#include "VertexArray.hpp"

class ProgramBundle {
    
public:
    ShaderProgram program;
    VertexArray va;
    
    ProgramBundle(ShaderProgram program, VertexArray va): program(program), va(va) {};
    
    void use() {
        program.use();
        va.use();
    }
};
