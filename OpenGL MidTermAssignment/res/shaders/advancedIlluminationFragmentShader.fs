#version 330

in vec3 tnorm;
in vec3 world_space_vertex;
in vec2 tex_coords;

out vec4 fragment_color;

uniform vec3 view_position;

// Might use texture if object have different material ?.
struct Material {
    vec3 ambiant_light_color;
    vec3 diffuse_light_color;
    vec3 specular_light_color;
    
    float specular_strength; // Shininess
};
uniform Material material;

uniform sampler2D object_texture;
uniform int texture_present;

/*********/
/* Light */
/*********/

// Default Configuration
float ambiant_strength = 0.25;
float specular_coefficient = 100.0;

/* Point Light light comming from a point */
struct PointLight {
    vec3 position;
    
    // Light Attenuation variables
    float constant;
    float linear;
    float quadratic;

    // 1, 1, 1 --> Standard Light, No effect
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_POINT_LIGHT_QTY 5
uniform int POINT_LIGHT_QTY = 0;
uniform PointLight pointLights[MAX_POINT_LIGHT_QTY];

// intensity is used for spotlight calculation elsewhere use 1.
vec3 computePointLight(PointLight light, vec3 normal, vec3 worldSpaceVertex, vec3 viewDirection, float intensity)
{
    vec3 lightDirection = normalize(light.position - worldSpaceVertex);

    // Ambiant
    vec3 ambient = light.ambient * material.ambiant_light_color * ambiant_strength;

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse_light_color;

    // Specular
    vec3 reflectionDirection = normalize(reflect(-lightDirection, normal)); // -lD ?
    float spec_dot_product = max(dot(viewDirection, reflectionDirection), 0.0);
    float spec = pow(spec_dot_product, specular_coefficient);
    vec3 specular = light.specular * spec * material.specular_light_color * material.specular_strength;
    
    // Attenuation
    float dist = length(light.position - worldSpaceVertex);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    
    return (ambient + diffuse * intensity + specular * intensity) * attenuation;
}

/* Direction light is a light coming from far away */
struct DirectionLight {
    vec3 direction;
  
    // 1, 1, 1 --> Standard Light, No effect
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionLight directionLight;
uniform int DIRECTION_LIGHT_PRESENT = 0;

vec3 computeDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);
    
    // Ambiant
    vec3 ambient = light.ambient * material.ambiant_light_color * ambiant_strength;
    
    // diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse_light_color;

    // specular shading
    vec3 reflectionDirection = normalize(reflect(-lightDirection, normal)); // -lD ?
    float spec_dot_product = max(dot(viewDirection, reflectionDirection), 0.0);
    float spec = pow(spec_dot_product, specular_coefficient);
    vec3 specular = light.specular * spec * material.specular_light_color * material.specular_strength;

    return (ambient + diffuse + specular);
}

/* Spot light */
struct SpotLight {
    vec3  direction;
    float cutOff;
    float outerCutOff;
    
    PointLight pointLight;
};

#define MAX_SPOT_LIGHT_QTY 5
uniform int SPOT_LIGHT_QTY = 0;
uniform SpotLight spotLights[MAX_SPOT_LIGHT_QTY];

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 worldSpaceVertex, vec3 viewDirection) {
    vec3 lightDirection = normalize(light.pointLight.position - worldSpaceVertex);
    float theta = dot(lightDirection, normalize(-light.direction));
    
    if (theta > light.outerCutOff) { // Inside the light cone
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        return computePointLight(light.pointLight, normal, worldSpaceVertex, viewDirection, intensity);
    }
    else { // Outside the light cone
        return vec3(0, 0, 0);
        return light.pointLight.ambient * material.ambiant_light_color * ambiant_strength;
    }
}

/* Fog */

uniform int useFog = 0;

const vec4 FOG_COLOR = vec4(0.85, 0.85, 0.85, 1.0);
const float FOG_START = 10.0;
const float FOG_STOP = 100.0;

/********/
/* MAIN */
/********/

void main() {
    /* Prepare Variables */
    vec3 normal = normalize(tnorm);
    vec3 viewDirection = normalize(view_position - world_space_vertex);

    /* Compute Ligts */
    vec3 computedDirectionLight = vec3(0.0, 0.0, 0.0);
    if (DIRECTION_LIGHT_PRESENT == 1) {
        computedDirectionLight = computeDirectionLight(directionLight, normal, viewDirection);
    }
    
    vec3 computedPointLights = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < POINT_LIGHT_QTY; i++) {
        computedPointLights += computePointLight(pointLights[i], normal, world_space_vertex, viewDirection, 1.0);
    }
    
    vec3 computedSpotLight = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < SPOT_LIGHT_QTY; i++) {
        computedSpotLight += computeSpotLight(spotLights[i], normal, world_space_vertex, viewDirection);
    }
    
    /* Combine Lights */
    vec3 light = computedDirectionLight + computedPointLights + computedSpotLight;
    fragment_color = vec4(light, 1.0);
    
    /* Handle Texture */
    if (texture_present == 1) {
        vec4 texture_color = texture(object_texture, tex_coords);
        fragment_color = fragment_color * texture_color;
    }
    
    /* Handle Fog */
    if (useFog == 1) {
        vec3 camFrag = world_space_vertex - view_position;
        float dist = abs(length(camFrag));

        float fogFactor = 0.0;
        if (dist >= FOG_START) {
            if (dist >= FOG_STOP) {
                fogFactor = 1.0;
            }
            else {
                fogFactor = (dist - FOG_START) / (FOG_STOP - FOG_START);
            }
        }
        fogFactor = (exp(fogFactor) - 1) / (exp(1) - 1);

        fragment_color = fogFactor * FOG_COLOR + (1.0 - fogFactor) * fragment_color;
    }
}
