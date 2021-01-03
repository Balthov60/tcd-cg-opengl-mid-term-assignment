//
//  BoidManager.hpp
//  OpenGL MidTermAssignment
//
//  Created by Balthazar Frolin on 09/12/2020.
//

#pragma once

#include "CGUtils.hpp"
#include "Transformation.hpp"

#include <vector>

class Boid;
class BoidManager
{
public:
    vector<Boid *> boids;
    vector<vec2> avoids;
    vector<vec2> nonBoundaryAvoids;

    // Boid Properties
    
    float globalScale = 0.1;
    
    float maxSpeed = 2 * globalScale;
    float friendRadius = 90 * globalScale;
    float crowdRadius = friendRadius / 1.25;
    float avoidRadius = 20 * globalScale;
    float coheseRadius = friendRadius;

    
    vec2 size = vec2(42.5, 370);
    
    BoidManager() {
        boids = vector<Boid *>();
        
        for (float i = 0; i <= size.x; i += 0.25) {
            for (float j = 0; j <= size.y; j += 0.25) {
                if (i == 0 || j == 0 || i == size.x || j == size.y ) {
                    avoids.push_back(vec2(i, j));
                }
                if (i == 21.25 && (int)j % 30 == 0) {
                    avoids.push_back(vec2(i, j));
                    nonBoundaryAvoids.push_back(vec2(j, i));
                }
            }
        }
        
        for (int i = 0; i < 100; i++) {
            addBoid();
        }
    }
    
    void update();
    void addBoid();
    
    vector<Transformation> getBoidsTransforms(Transformation parentTransform);
    vector<Transformation> getAvoidsTransforms(Transformation parentTransform);

};

static int boid_qty = 0;

class Boid
{
public:
    int boidNum;
    
    BoidManager * manager;
    
    // Use of this ? : init = random(10)
    float thinkTimer;
    float nextUpdate;
    
    vec2 position;
    vec2 direction;
    
    vector<Boid *> friends;
    
    Boid(vec2 position, BoidManager * manager): position(position), manager(manager) {
        direction = glm::normalize(position);
        
        thinkTimer = ((float) (rand() % 100)) / 400.0;
        nextUpdate = thinkTimer;
        boidNum = boid_qty;
        boid_qty++;
    }
    
    void update() {
        thinkTimer += ControlsManager::deltaTime;
        // Wraping
        
        if (thinkTimer >= 0.25 ) {
            thinkTimer = 0;
            nextUpdate = 0;
            updateFriends();
            
            flock();
            position += direction;
        }
        else if (thinkTimer >= nextUpdate) {
            nextUpdate += 0.05;
            flock();
            position += direction;
        }
    }
    
    Transformation getTransform() {
        vec3 translate = vec3(position, 0);
        
        float angle = degrees(atan2(direction.y, direction.x));
        vec3 rotation = vec3(0, 0, angle + 90);
        
        return Transformation(translate, rotation);
    }
    
private:
    
    void updateFriends() {
        friends.clear();
        
        for (Boid * boid : manager->boids) {
            if (boid != this && isFriend(boid)) {
                friends.push_back(boid);
            }
        }
        
        // Debug
        if (boidNum == 0) {
            printf("updated friends : %d, radius : %f \n", friends.size(), manager->friendRadius);
        }
    }
    bool isFriend(Boid * boid) {
        return abs(boid->position.x - this->position.x) < manager->friendRadius
            && abs(boid->position.y - this->position.y) < manager->friendRadius;
    }
    
    void flock() {
        vec2 averageDirection = getAverageDirection();
        vec2 cohesionDirection = getCohesionDirection();

        vec2 avoidFriendsDirection = getAvoidFriendsDirection();
        vec2 avoidAvoidsDirection = getAvoidAvoidsDirection();

        vec2 noiseDirection = vec2(((float)(rand() % 2000)) / 1000.0 - 1.0, ((float)(rand() % 2000)) / 1000.0 - 1.0);
        
        direction += averageDirection + avoidFriendsDirection + vec2(3 * avoidAvoidsDirection.x, 3 * avoidAvoidsDirection.y) + vec2(0.1 * noiseDirection.x, 0.1 * noiseDirection.y) + cohesionDirection;
        
        direction = glm::clamp(direction, -manager->maxSpeed, manager->maxSpeed);
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
                vec2 dirr = boid->direction;
                dirr = glm::normalize(direction);
                dirr /= distance;
                
                sum += dirr;
                count++;
            }
        }
        
        if (count > 0) {
            sum /= (float) count; // Commented in code
        }
        
        return sum;
    }
    
    /**
        Avoid collision with friends
     */
    vec2 getAvoidFriendsDirection() {
        vec2 sum(0.0, 0.0);
        int count = 0;

        for (Boid * boid : friends) {
            float distance = glm::distance(position, boid->position);
            
            if ((distance > 0) && (distance < manager->crowdRadius)) {
                vec2 diff = position - boid->position;
                diff = glm::normalize(diff);
                diff /= distance;
                
                sum += diff;
                count++;
            }
        }
        if (count > 0) {
            // sum /= (float) count; // Commented in code
        }
        
        return sum;
     }
    
    /**
        Avoid Obstacles
     */
    vec2 getAvoidAvoidsDirection() {
        vec2 sum(0.0, 0.0);
        int count = 0;

        for (vec2 avoid : manager->avoids) {
            float distance = glm::distance(position, avoid);
            
            if ((distance > 0) && (distance < manager->avoidRadius)) {
                vec2 difference = position - avoid;
                difference = glm::normalize(difference);
                difference /= distance;
                
                sum += difference;
                count++;
            }
        }
        if (count > 0) {
            sum /= (float)count;
        }
        
        return sum;
    }
    
    /**
        Keep a near position
     */
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
            sum /= (float) count;
            sum -= position;
            
            // float magnitude = glm::length(sum);
            // sum = vec2(sum.x * 0.05 / magnitude, sum.y * 0.05 / magnitude);
        }
        
        return sum;
    }
};
