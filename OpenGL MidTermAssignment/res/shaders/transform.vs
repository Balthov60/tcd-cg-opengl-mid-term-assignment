#version 330 core

in vec3 position;
in vec4 color;
in vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec4 vertexColor;
out vec2 texture_coords;

void main()
{
    gl_Position = projection * view * transform * vec4(position.x, position.y, position.z, 1.0);
    vertexColor = color;
    texture_coords = texCoords;
}

