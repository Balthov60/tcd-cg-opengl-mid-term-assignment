#version 330 core

in vec3 position;
in vec4 color;

out vec4 vertexColor;

void main()
{
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  vertexColor = color;
}
