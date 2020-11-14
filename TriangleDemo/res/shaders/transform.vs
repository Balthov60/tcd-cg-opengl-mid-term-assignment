#version 330 core

in vec3 position;
in vec4 color;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec4 vertexColor;

void main()
{
  gl_Position = projection * view * transform * vec4(position.x, position.y, position.z, 1.0);
  vertexColor = color;
}

