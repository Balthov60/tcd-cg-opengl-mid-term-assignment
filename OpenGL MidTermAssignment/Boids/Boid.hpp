//
//  Boid.hpp
//  OpenGL MidTermAssignment
//
//  Created by Balthazar Frolin on 09/12/2020.
//

#pragma once

// #include "CGUtils.hpp"
// #include "BoidManager.hpp"

class Boid
{
public:
    BoidManager * manager;
    
    // Use of this ? : init = random(10)
    int thinkTimer = 4;
    
    vec2 position;
    vec2 direction;
    
    vector<Boid *> friends;
    
    Boid(vec2 position, BoidManager * manager): position(position), manager(manager) { }
    
    void update() {
        thinkTimer = (thinkTimer + 1) % 5;
        position = vec2(position.x % manager->size.x, position.y % manager->size.y);

        if (thinkTimer == 0 ) {
            updateFriends();
        }
        flock();
        position += direction;
    }
    
    void updateFriends() {
        friends.clear();
        
        for (Boid * boid : manager->boids) {
            if (boid != this && isFriend(boid)) {
                friends.push_back(boid);
            }
        }
    }
    
    void flock () {
        vec2 averageDirection = getAverageDirection();
        vec2 avoidDirection = getAvoidDirection();
        vec2 cohesionDirection = getCohesionDirection();

        // vec2 avoidAvoidsDirection = 3 * getAvoidAvoids();

        vec2 noiseDirection = vec2(rand() % 2 - 1, rand() % 2 - 1);

        // Possible to create option or change coefficient
        direction += averageDirection + avoidDirection + /* vec2(3, 3) * avoidObjects +*/ vec2(0.1 * noiseDirection.x, 0.1 * noiseDirection.y) + cohesionDirection;
        direction = glm::clamp(direction, -manager->maxSpeed, manager->maxSpeed);
    }
    
private:
    
    bool isFriend(Boid * boid) {
        return abs(boid->position.x - this->position.x) < manager->friendRadius
            && abs(boid->position.y - this->position.y) < manager->friendRadius;
    }
    
    /**
     Get Average direction of all friends
     */
    vec2 getAverageDirection() {
        vec2 sum(0.0, 0.0);
        int count = 0;

        for (Boid * boid : friends) {
            float distance = glm::distance(position, boid->position);
            
            if ((distance > 0) && (distance < manager->friendRadius)) {
                vec2 direction = boid->direction;
                direction = glm::normalize(direction);
                direction /= distance;
                
                sum += direction;
                count++;
            }
        }
        
        if (count > 0) {
            sum /= count; // Commented in code
        }
        
        return sum;
    }
    
    /**
     ??
     */
    vec2 getAvoidDirection() {
        vec2 sum(0.0, 0.0);
        int count = 0;

        for (Boid * boid : friends) {
            float distance = glm::distance(position, boid->position);
            
            if ((distance > 0) && (distance < manager->crowdRadius)) {
                vec2 direction = boid->direction;
                direction = glm::normalize(direction);
                direction /= distance;
                
                sum += direction;
                count++;
            }
        }
        if (count > 0) {
            sum /= count; // Commented in code
        }
        
        return sum;
     }
    
    vec2 getCohesionDirection() {
        vec2 sum(0.0, 0.0);
        int count = 0;
                        
        for (Boid * boid : friends) {
            float distance = glm::distance(position, boid->position);
            if ((distance > 0) && (distance < manager->coheseRadius)) {
                sum += boid->position;
                count++;
            }
        }
        if (count > 0) {
            sum /= count;
            return sum - position;
        }
        else {
            return vec2(0, 0);
        }
    }
};
