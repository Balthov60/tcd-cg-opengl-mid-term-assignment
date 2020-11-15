//
//  Animation.hpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 11/11/2020.
//

#pragma once
#include "CGUtils.hpp"
#include "Transformation.hpp"

enum ANIMATION_DIRECTION {
    ANIM_FORWARD,
    ANIM_BACKWARD,
};

struct TranslationAnimationData {
    vec3 start;
    vec3 end;
    
    TranslationAnimationData() {}
    TranslationAnimationData(vec3 start, vec3 end) : start(start), end(end) {}
};
struct RotationAnimationData {
    quat start;
    quat end;
    
    RotationAnimationData() {}
    
    RotationAnimationData(vec3 startAngle, vec3 endAngle) {
        Transformation start(vec3(0, 0, 0), startAngle);
        Transformation end(vec3(0, 0, 0), endAngle);
        
        this->start = quat_cast(start.matrix);
        this->end = quat_cast(end.matrix);
    }
    RotationAnimationData(quat start, quat end) : start(start), end(end) {}
    RotationAnimationData(Transformation start, Transformation end) {
        this->start = quat_cast(start.matrix);
        this->end = quat_cast(end.matrix);
    }

};

class AnimationState
{
public:
    bool finished = false;
    
    TranslationAnimationData position;
    RotationAnimationData rotation;
    
    AnimationState(TranslationAnimationData position, RotationAnimationData rotation, float duration)
    : position(position), rotation(rotation), duration(duration) {
        transform = Transformation();
        transform.translate(position.start);
        transform.quaternionRotate(rotation.start);
    }
    
    AnimationState(AnimationState previousState, vec3 endPosition, vec3 endRotation, float duration)
    : duration(duration) {
        position = TranslationAnimationData(previousState.position.end, endPosition);
        
        Transformation endRotationTransform(vec3(0, 0, 0), endRotation);
        quat endRotationQuaternion = quat_cast(endRotationTransform.matrix);
        rotation = RotationAnimationData(previousState.rotation.end, endRotationQuaternion);

        initTransform();
    }
    
    Transformation updateAndGetTransform(ANIMATION_DIRECTION direction) {
        if (updateCurrentTime()) {
            float progression = getProgression(direction);
            transform = Transformation();
            
            vec3 translationInterpolation = mix(position.start, position.end, progression);
            transform.translate(translationInterpolation);
            
            quat rotationInterpolation = slerp(rotation.start, rotation.end, progression);
            transform.quaternionRotate(rotationInterpolation);
        }
        
        return transform;
    }
    
    void reset() {
        currentTime = 0;
        finished = false;
    }
    
private:
    
    float duration;
    float currentTime = 0;
    
    Transformation transform;
    
    void initTransform() {
        transform = Transformation();
        transform.translate(position.start);
        transform.quaternionRotate(rotation.start);
    }
    
    bool updateCurrentTime() {
        if (finished) {
            return false;
        }
        
        currentTime += ControlsManager::deltaTime;
        
        if (currentTime >= duration) {
            finished = true;
        }
        
        return true;

    }
    
    float getProgression(ANIMATION_DIRECTION direction) {
        if (direction == ANIM_FORWARD) {
            return currentTime / duration;
        }
        else {
            return 1 - (currentTime / duration);
        }
    }
};
