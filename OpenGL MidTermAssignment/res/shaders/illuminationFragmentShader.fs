#version 330

in vec3 tnorm;
in vec3 world_space_vertex;
in vec2 tex_coords;

out vec4 fragment_color;

uniform vec3 view_position;

uniform vec3 light_position;

// Material
struct Material {
    vec3 ambiant_light_color;
    vec3 diffuse_light_color;
    vec3 specular_light_color;
    
    float specular_strength; // Shininess
};
uniform Material material;

uniform sampler2D object_texture;
uniform int texture_present;

// Ambiant
float ambiant_strength = 1.0;

// Specular
float specular_coefficient = 100.0;

void main() {
    // Ambiant
    vec3 ambiant = ambiant_strength * material.ambiant_light_color;
    
    // Diffuse
    vec3 norm = normalize(tnorm);
    vec3 light_direction = normalize(light_position - world_space_vertex);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * material.diffuse_light_color;
    
    // Specular
    vec3 view_direction = normalize(view_position - world_space_vertex);
    vec3 reflection_direction = normalize(reflect(light_direction, tnorm));
    
    float specular_dot_product = max(dot(view_direction, reflection_direction), 0.0);
    float spec = pow(specular_dot_product, specular_coefficient);
    vec3 specular = spec * material.specular_strength * material.specular_light_color;
    
    // Final color
    vec3 light = vec3(specular + ambiant + diffuse);
    fragment_color = vec4(light, 1.0);
    if (texture_present == 1) {
        vec4 texture_color = texture(object_texture, tex_coords);
        fragment_color = fragment_color + texture_color;
    }
}
