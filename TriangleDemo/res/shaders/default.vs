#version 330 core

in vec3 position;
in vec4 color;

uniform mat4 PROJECTION;
uniform mat4 VIEW;

out vec4 vertexColor;

void main()
{
  gl_Position = PROJECTION * VIEW * vec4(position.x, position.y, position.z, 1.0);
  vertexColor = color;
}
