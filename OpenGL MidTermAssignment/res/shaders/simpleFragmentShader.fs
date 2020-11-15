#version 330

in vec3 LightIntensity;
out vec4 color;
void main(){
	color = vec4 (LightIntensity, 1.0);
}
