//
//  Animation.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 11/11/2020.
//
#pragma once
#include "AnimationState.hpp"
#include "CGUtils.hpp"
#include "Transformation.hpp"
#include <map>
#include <vector>

enum ANIMATION_TYPE {
    NORMAL,
    LOOP,
    BOUNCE,
    BOUNCE_PAUSE,
};
const float BOUNSE_PAUSE_DURATION = 1.5;

class Animation
{
public:
    
    Animation(string synchroString = "DEFAULT", ANIMATION_TYPE type = BOUNCE_PAUSE);
    
    void addState(AnimationState state, float startDelay);
    
    Transformation updateAndGetTransform();
    
private:
    /* Animation Data */
    vector<AnimationState> states;
    vector<float> statesStartTime;

    float currentTime = 0;
    float currentStateIndex = 0;
    
    ANIMATION_TYPE type = BOUNCE_PAUSE;
    ANIMATION_DIRECTION direction = ANIM_FORWARD;
    
    /* Synchronisation with other animation */
    static map<string, vector<bool>> AnimationSynchronisation;
    string synchroString;
    unsigned long synchroPlace;
    bool lockNextFrame = false;
    
    int getNextStateIndex() {
        if (direction == ANIM_FORWARD) {
            return currentStateIndex + 1;
        }
        else {
            return currentStateIndex - 1;
        }
    }
    
    void invertDirection() {
        if (direction == ANIM_FORWARD) {
            direction = ANIM_BACKWARD;
        }
        else {
            direction = ANIM_FORWARD;
        }
    }
    
    float getNextStateStartTime() {
        if (direction == ANIM_FORWARD) {
            return statesStartTime[getNextStateIndex()];
        }
        else {
            return statesStartTime[currentStateIndex];
        }
    }
    
    bool synchroWithOther() {
        AnimationSynchronisation.at(synchroString)[synchroPlace] = true;
        
        for (bool entry : AnimationSynchronisation.at(synchroString)) {
            if (!entry) {
                return false;
            }
        }
        
        lockNextFrame = true;
        return true;
    }
    
};
