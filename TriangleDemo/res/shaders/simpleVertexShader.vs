#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


out vec3 LightIntensity;

vec4 LightPosition = vec4 (10.0, 10.0, 4.0, 1.0); // Light position in world coords.
vec3 Kd = vec3 (0.0, 0.8, 0.0); // green diffuse surface reflectance
vec3 Ld = vec3 (1.0, 1.0, 1.0); // Light source intensity


uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main(){

  mat4 viewTransform = view * transform;
  mat3 normalMatrix =  mat3(viewTransform);
  // Convert normal and position to eye coords
  // Normal in view space
  vec3 tnorm = normalize(normalMatrix * normal);
  // Position in view space
  vec4 eyeCoords = viewTransform * vec4(position,1.0);
  //normalised vector towards the light source
  vec3 s = normalize(vec3(LightPosition - eyeCoords));
  
  // The diffuse shading equation, dot product gives us the cosine of angle between the vectors
  LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );
  
  // Convert position to clip coordinates and pass along
  gl_Position = projection * view * transform * vec4(position, 1.0);
}


  
