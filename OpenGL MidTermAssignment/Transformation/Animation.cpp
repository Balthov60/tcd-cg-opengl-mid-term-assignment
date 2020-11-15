//
//  Animation.cpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 14/11/2020.
//

#include "Animation.hpp"

map<string, vector<bool>> Animation::AnimationSynchronisation {
    {string("default"), vector<bool>()}
};

Animation::Animation(string synchroString, ANIMATION_TYPE type): synchroString(synchroString), type(type) {
    AnimationSynchronisation[synchroString].push_back(false);
    synchroPlace = AnimationSynchronisation[synchroString].size() - 1;
}

void Animation::addState(AnimationState state, float startDelay) {
    states.push_back(state);
    statesStartTime.push_back(startDelay);
}

Transformation Animation::updateAndGetTransform() {
    AnimationState &state = states[currentStateIndex];
    Transformation transform = state.updateAndGetTransform(direction);
    
    if (state.finished) {
        int nextStateIndex = getNextStateIndex();
        if (states.size() > nextStateIndex) {
            if (lockNextFrame) {
                AnimationSynchronisation[synchroString][synchroPlace] = false;
                lockNextFrame = false;
            }
            currentTime += ControlsManager::deltaTime;
            float nextStartTime = getNextStateStartTime();
            
            if (currentTime >= nextStartTime) {
                currentTime = 0;
                currentStateIndex = nextStateIndex;
                
                state.reset();
            }
        }
        else {
            if (type == LOOP) {
                currentTime += ControlsManager::deltaTime;
                float nextStartTime = statesStartTime[0];

                if (currentTime >= nextStartTime) {
                    currentTime = 0;
                    currentStateIndex = 0;
                    
                    state.reset();
                }
            }
            else if (type == BOUNCE || type == BOUNCE_PAUSE) {
                if (direction == ANIM_BACKWARD && !lockNextFrame) {
                    synchroWithOther();
                }
                else {
                    currentTime += ControlsManager::deltaTime;
                    
                    float nextStartTime = 0;
                    if (direction == ANIM_BACKWARD && type == BOUNCE_PAUSE) {
                        nextStartTime += BOUNSE_PAUSE_DURATION;
                    }
                    if (currentTime >= nextStartTime) {
                        invertDirection();
                        
                        currentTime = 0;
                        
                        state.reset();
                    }
                }
            }
        }
    }
    return transform;
}
