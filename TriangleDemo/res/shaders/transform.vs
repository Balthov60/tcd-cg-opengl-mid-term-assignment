#version 330 core

in vec3 position;
in vec4 color;

uniform mat4 TRANSFORM_MATRIX;

out vec4 vertexColor;

void main()
{
  gl_Position = TRANSFORM_MATRIX * vec4(position.x, position.y, position.z, 1.0);
  vertexColor = color;
}

