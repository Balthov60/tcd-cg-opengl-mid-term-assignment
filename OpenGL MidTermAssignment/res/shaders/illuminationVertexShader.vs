#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coords;

out vec3 world_space_vertex;
out mat3 normal_matrix;
out vec3 tnorm;
out vec2 tex_coords;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {    
    world_space_vertex = vec3(transform * vec4(position, 1.0f));
    tnorm = mat3(transpose(inverse(transform))) * normal;
    
    gl_Position = projection * view * transform * vec4(position, 1.0f);
    tex_coords = texture_coords;
}


  
