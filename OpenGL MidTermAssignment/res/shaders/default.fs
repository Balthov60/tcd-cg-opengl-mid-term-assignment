#version 330 core
in vec4 vertexColor;
in vec2 texture_coords;
out vec4 fragmentColor;

uniform sampler2D object_texture;
uniform int texture_present;

void main()
{
     fragmentColor = vertexColor;
    if (texture_present == 1) {
        vec4 texture_color = texture(object_texture, texture_coords);
        fragmentColor = texture_color;
    }
}
