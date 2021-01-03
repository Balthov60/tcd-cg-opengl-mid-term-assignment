//
//  BoidManager.cpp
//  OpenGL MidTermAssignment
//
//  Created by Balthazar Frolin on 09/12/2020.
//

#include "BoidManager.hpp"

void BoidManager::addBoid() {
    vec2 position = vec2(rand() % (int)size.x, rand() % (int)size.y);
    printf("init %f %f\n", position.x, position.y);
    Boid * boid = new Boid(position, this);
    boids.push_back(boid);
}

void BoidManager::update() {
    for (Boid * boid : boids) {
        boid->update();
    }
}

vector<Transformation> BoidManager::getBoidsTransforms(Transformation parentTransform) {
    vector<Transformation> transforms;
    
    for (Boid * boid : boids) {
        Transformation transform = boid->getTransform();
        transform = parentTransform * transform;
        transform.scale(0.05);
        transforms.push_back(transform);
    }
    return transforms;
}

vector<Transformation> BoidManager::getAvoidsTransforms(Transformation parentTransform) {
    vector<Transformation> transforms;
    
    for (vec2 avoid : nonBoundaryAvoids) {
        Transformation transform;
        transform.translate(vec3(avoid.y, avoid.x, 0.0));
        transform = parentTransform * transform;
        transforms.push_back(transform);
    }
    return transforms;
}
