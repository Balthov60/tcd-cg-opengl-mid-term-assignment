//
//  Lights.;
//  OpenGL MidTermAssignment
//
//  Created by Balthazar Frolin on 11/12/2020.
//

#pragma once

struct DirectionLight {
    vec3 direction;
  
    vec3 ambient = vec3(1, 1, 1);
    vec3 diffuse = vec3(1, 1, 1);
    vec3 specular = vec3(1, 1, 1);
    
    DirectionLight(vec3 direction) : direction(direction) {};
};

#define MAX_POINT_LIGHT_QTY 5
struct PointLight {
    vec3 position;
    
    // Light Attenuation variables
    float constant = 1.0;
    float linear = 0.007;
    float quadratic = 0.0002;

    vec3 ambient = vec3(1, 1, 1);
    vec3 diffuse = vec3(1, 1, 1);
    vec3 specular = vec3(1, 1, 1);
    
    PointLight(vec3 position) : position(position) {};
};

/**
 Distance : const, linear, quadratic
 7    1.0    0.7    1.8
 13    1.0    0.35    0.44
 20    1.0    0.22    0.20
 32    1.0    0.14    0.07
 50    1.0    0.09    0.032
 65    1.0    0.07    0.017
 100    1.0    0.045    0.0075
 160    1.0    0.027    0.0028
 200    1.0    0.022    0.0019
 325    1.0    0.014    0.0007
 600    1.0    0.007    0.0002
 3250    1.0    0.0014    0.000007
 */

#define MAX_SPOT_LIGHT_QTY 5
struct SpotLight {
    vec3 direction;
    float cutOff = 0.97;
    float outerCutOff = 0.90;
    
    PointLight pointLight;
    
    SpotLight(vec3 position, vec3 direction) : pointLight(position), direction(direction) {};
};
