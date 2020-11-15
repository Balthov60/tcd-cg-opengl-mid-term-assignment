//
//  SpiderModelUtils.cpp
//  TriangleDemo
//
//  Created by Balthazar Frolin on 14/11/2020.
//

#pragma once
#include "CGUtils.hpp"
#include "Transformation.hpp"
#include "HierarchicalModel.hpp"
#include "Animation.hpp"

Model * spiderBody;
// Left Leg;
Model * spiderLeftUpLeg;
Model * spiderLeftMiddleLeg;
Model * spiderLeftDownLeg;
// Right Leg;
Model * spiderRightUpLeg;
Model * spiderRightMiddleLeg;
Model * spiderRightDownLeg;

float FRAME_BY_SECOND = 35.0;
float frameToSecond(float frame) {
    return frame / FRAME_BY_SECOND;
}
float stateDuration[] = {
    10.0,
    10.0,
    10.0,
    5.0,
    5.0,
    4.0,
    3.0,
    3.0
};
float getStateDuration(int i) {
    return frameToSecond(stateDuration[i]);
};

int spiderQty = 0;
string ANIMATION_SYNCHRO_STRING = "";

bool modelLoaded = false;
void initModels() {
    if (modelLoaded) {
        return;
    }
    spiderBody = new Model("spider/spider_body.dae");

    spiderLeftUpLeg = new Model("spider/spider_left_leg_up.dae");
    spiderLeftMiddleLeg = new Model("spider/spider_left_leg_middle.dae");
    spiderLeftDownLeg = new Model("spider/spider_left_leg_down.dae");
    
    spiderRightUpLeg = new Model("spider/spider_right_leg_up.dae");
    spiderRightMiddleLeg = new Model("spider/spider_right_leg_middle.dae");
    spiderRightDownLeg = new Model("spider/spider_right_leg_down.dae");
    
    modelLoaded = true;
}

// Right Legs Transform
const Transformation leftLegsTransform[] = {
    Transformation(vec3(0.933725, 2.55892, 1.38035), vec3(90, 0, -17)),
    Transformation(vec3(1.84781, 4.73044, 1.43405), vec3(90, 0, -2.01)),
    Transformation(vec3(1.62169, 7.0024, 1.43405), vec3(90, 0, 11.1)),
    Transformation(vec3(1.48775, 9.08076, 1.50708), vec3(90, 0, 20.7)),
};

const Transformation rightLegsTransform[] = {
    Transformation(vec3(-0.933725, 2.55892, 1.38035), vec3(90, 0, -163)),
    Transformation(vec3(-1.84781, 4.73044, 1.43405), vec3(90, 0, 178)),
    Transformation(vec3(-1.62169, 7.0024, 1.43405), vec3(90, 0, 169)),
    Transformation(vec3(-1.48775, 9.08076, 1.50708), vec3(90, 0, 159)),
};

// Animations

    //Body Animation
Animation * createBodyAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    AnimationState firstState(
       TranslationAnimationData(vec3(0, 0, 5.48044), vec3(0, 0, 6.35771)),
       RotationAnimationData(vec3(0, 0, 0), vec3(-10.6934, 0.19119, -0.805793)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);
    
    AnimationState secondState(firstState, vec3(0, 0, 6.35771), vec3(-17.5, 0.0949, -0.823), getStateDuration(1));
    animation->addState(secondState, 0);

    AnimationState thirdState(secondState, vec3(0, 0, 8.96822), vec3(-31.1409, -0.102379, -0.82181), getStateDuration(2));
    animation->addState(thirdState, 0);

    AnimationState fourthState(thirdState, vec3(0, 0, 8.96822), vec3(-45.7063, -0.305773, -0.76965), getStateDuration(3));
    animation->addState(fourthState, 0);

    // getStateDuration(4)
    
    AnimationState sixthState(fourthState, vec3(0, 0, 8.96822), vec3(-33.5636, -0.137028, -0.816748), getStateDuration(5));
    animation->addState(sixthState, getStateDuration(4));

    AnimationState seventhState(sixthState, vec3(0, 0, 8.96822), vec3(-45.7063, -0.305773, -0.76965), getStateDuration(6));
    animation->addState(seventhState, 0);

    AnimationState eighthState(seventhState, vec3(0, 0, 8.96822), vec3(-33.5636, -0.137028, -0.816748), getStateDuration(7));
    animation->addState(eighthState, 0);
    
    return animation;
}

    // Left Leg Animations
        // Up Leg
Animation * createFirstLeftLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    AnimationState firstState(
       TranslationAnimationData(vec3(0.933725, 2.47272, 1.38035), vec3(0.933725, 2.47272, 1.38035)),
       RotationAnimationData(vec3(90, 0, -17), vec3(90, 0, -17)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);
    
    AnimationState secondState(firstState, vec3(0.933725, 2.47272, 1.38035), vec3(73, -4.32521, -34), getStateDuration(1));
    animation->addState(secondState, 0);

    AnimationState thirdState(secondState, vec3(0.933725, 2.47272, 1.38035), vec3(64, -5.78629, -63), getStateDuration(2));
    animation->addState(thirdState, 0);
    // getStateDuration(3)
    // getStateDuration(4)
    
    AnimationState sixthState(thirdState, vec3(0.933725, 2.47272, 1.38035), vec3(47, 4.7923, -75), getStateDuration(5));
    animation->addState(sixthState, getStateDuration(3) + getStateDuration(4));

    AnimationState seventhState(sixthState, vec3(0.933725, 2.47272, 1.38035), vec3(64, -5.78629, -63), getStateDuration(6));
    animation->addState(seventhState, 0);

    AnimationState eighthState(seventhState, vec3(0.933725, 2.47272, 1.38035), vec3(47, 4.7923, -75), getStateDuration(7));
    animation->addState(eighthState, 0);
    
    return animation;
}
Animation * createSecondLeftLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    // First + Second
    AnimationState firstState(
       TranslationAnimationData(vec3(1.84781, 4.73044, 1.43405), vec3(1.84781, 4.73044, 1.43405)),
       RotationAnimationData(vec3(90, 0, -2.01391), vec3(90, 0, -2.01391)),
       getStateDuration(0) + getStateDuration(1)
    );
    animation->addState(firstState, 0);

    AnimationState thirdState(firstState, vec3(1.84781, 4.73044, 1.43405), vec3(47, -5.34221, -34.02), getStateDuration(2));
    animation->addState(thirdState, 0);
    // getStateDuration(3)
    
    AnimationState fifthState(thirdState, vec3(1.84781, 4.73044, 1.43405), vec3(34, 10.1895, -54), getStateDuration(4));
    animation->addState(fifthState, getStateDuration(3));
    
    AnimationState sixthState(fifthState, vec3(1.84781, 4.73044, 1.43405), vec3(33, 14.281, -70), getStateDuration(5));
    animation->addState(sixthState, 0);

    AnimationState seventhState(sixthState, vec3(1.84781, 4.73044, 1.43405), vec3(34, 10.1895, -54), getStateDuration(6));
    animation->addState(seventhState, 0);

    AnimationState eighthState(seventhState, vec3(1.84781, 4.73044, 1.43405), vec3(33, 14.281, -70), getStateDuration(7));
    animation->addState(eighthState, 0);
    
    return animation;
}
Animation * createThirdLeftLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    AnimationState firstState(
       TranslationAnimationData(vec3(1.62169, 7.0024, 1.43405), vec3(1.62167, 7.00705, 1.4272)),
       RotationAnimationData(vec3(90, 0, 11.0524), vec3(110.036, -3.58279, 10.5403)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);
    // getStateDuration(1)

    // Finally use 2 time frame for this
    AnimationState fourthState(firstState, vec3(1.62158, 7.01634, 1.42138), vec3(130, 2.23032, -11.9), getStateDuration(2) + getStateDuration(3));
    animation->addState(fourthState, getStateDuration(1));
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)
    
    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(1.62158, 7.01634, 1.42138), vec3(130, 2.23032, -11.9), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));
    
    return animation;
}
Animation * createFourthLeftLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    AnimationState firstState(
       TranslationAnimationData(vec3(0.626824, 9.08076, 1.43405), vec3(0.626767, 9.08677, 1.48404)),
       RotationAnimationData(vec3(90, 0, 20.6568), vec3(108.142, -6.45088, 19.7401)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);
    
    AnimationState secondState(firstState, vec3(0.626743, 9.08887, 1.48332), vec3(113.89, -8.37603, 19.0325), getStateDuration(1));
    animation->addState(secondState, 0);

    // AnimationState thirdState(secondState, vec3(0.626702, 9.09227, 1.48263), vec3(123.006, -11.2176, 17.5342), getStateDuration(2));
    // animation->addState(thirdState, 0);
    
    AnimationState fourthState(secondState, vec3(0.626611, 9.09903, 1.48293), vec3(130.017, -35.0532, 28.3706), getStateDuration(2) + getStateDuration(3));
    animation->addState(fourthState, 0);
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)
    
    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(0.626611, 9.09903, 1.48293), vec3(130.017, -35.0532, 28.3706), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));
    
    return animation;
}

Animation * getLeftLegAnimation(int i) {
    switch (i) {
        case 1:
            return createFirstLeftLegAnimation();
        case 2:
            return createSecondLeftLegAnimation();
        case 3:
            return createThirdLeftLegAnimation();
        case 4:
            return createFourthLeftLegAnimation();
        default:
            return NULL;
    }
};

        // Middle Leg
Animation * createThirdLeftMiddleLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    // Useless State (no implementation of initial position)
    // State 1 + 2
    AnimationState firstState(
       TranslationAnimationData(vec3(8.70222, 2.23882, 0.35562), vec3(8.70222, 2.23882, 0.35562)),
       RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
       getStateDuration(0) + getStateDuration(1)
    );
    animation->addState(firstState, 0);

    AnimationState thirdState(firstState, vec3(8.70222, 2.23882, 0.35562), vec3(0.729216, 0.842941, -4.2042), getStateDuration(2));
    animation->addState(thirdState, 0);
    
    AnimationState fourthState(thirdState, vec3(8.70222, 2.23882, 0.35562), vec3(12.2, -12.4629, -1), getStateDuration(3));
    animation->addState(fourthState, 0);
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)
    
    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(8.70222, 2.23882, 0.35562), vec3(12.2, -12.4629, -1), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));
    
    return animation;
}
Animation * createFourthLeftMiddleLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    // Useless State (no implementation of initial position)
    AnimationState firstState(
       TranslationAnimationData(vec3(8.70222, 2.23882, 0.35562), vec3(8.70222, 2.23882, 0.35562)),
       RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);
    
    AnimationState secondState(firstState, vec3(8.7089, 2.21044, 0.337658), vec3(13.0339, -0.454084, 4.69101), getStateDuration(1));
    animation->addState(secondState, 0);

    // getStateDuration(2)
    // getStateDuration(3)
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)
    
    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(secondState, vec3(8.7089, 2.21044, 0.337658), vec3(13.0339, -0.454084, 4.69101), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(2) + getStateDuration(3) + getStateDuration(4) + getStateDuration(5) + getStateDuration(6));
    
    return animation;
}

Animation * getLeftMiddleLegAnimation(int i) {
    switch (i) {
        case 3:
            return createThirdLeftMiddleLegAnimation();
        case 4:
            return createFourthLeftMiddleLegAnimation();
        default:
            return NULL;
    }
};

// Down Leg
Animation * createThirdLeftDownLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    // Useless State (no implementation of initial position)
    AnimationState firstState(
        TranslationAnimationData(vec3(5.06459, -2.83332, 0.526954), vec3(5.06459, -2.83332, 0.526954)),
        RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
        getStateDuration(0)
    );
    animation->addState(firstState, 0);

    AnimationState secondState(firstState, vec3(5.06459, -2.83332, 0.526954), vec3(-2.44578, 1.02513, 14.0442), getStateDuration(1));
    animation->addState(secondState, 0);

    // getStateDuration(2)

    AnimationState fourthState(secondState, vec3(5.06459, -2.83332, 0.526954), vec3(-10.5309, -2.07191, -6.91), getStateDuration(3));
    animation->addState(fourthState, getStateDuration(2));
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)

    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(5.06459, -2.83332, 0.526954), vec3(-10.5309, -2.07191, -6.91), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));

    return animation;
}

Animation * getLeftDownLegAnimation(int i) {
    switch (i) {
        case 3:
            return createThirdLeftDownLegAnimation();
        default:
            return NULL;
    }
};

    // Right Leg Animations
        // Up Leg
Animation * createFirstRightLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    AnimationState firstState(
       TranslationAnimationData(vec3(-0.933725, 2.55892, 1.38035), vec3(-0.933725, 2.55892, 1.38035)),
       RotationAnimationData(vec3(90, 0, -163), vec3(90, 0, -163)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);

    AnimationState secondState(firstState, vec3(-0.933725, 2.55892, 1.38035), vec3(106.108, -4.32521, -146.009), getStateDuration(1));
    animation->addState(secondState, 0);

    AnimationState thirdState(secondState, vec3(-0.933725, 2.47272, 1.38035), vec3(125.568, -5.78629, -116.955), getStateDuration(2));
    animation->addState(thirdState, 0);
    // getStateDuration(3)
    // getStateDuration(4)

    AnimationState sixthState(thirdState, vec3(-0.933725, 2.55892, 1.38035), vec3(133.16, 4.7923, -105.11), getStateDuration(5));
    animation->addState(sixthState, getStateDuration(3) + getStateDuration(4));

    AnimationState seventhState(sixthState, vec3(-0.933725, 2.47272, 1.38035), vec3(125.568, -5.78629, -116.955), getStateDuration(6));
    animation->addState(seventhState, 0);

    AnimationState eighthState(seventhState, vec3(-0.933725, 2.55892, 1.38035), vec3(133.16, 4.7923, -105.11), getStateDuration(7));
    animation->addState(eighthState, 0);

    return animation;
}
Animation * createSecondRightLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    // First + Second
    AnimationState firstState(
       TranslationAnimationData(vec3(-1.84781, 4.73044, 1.43405), vec3(-1.84781, 4.73044, 1.43405)),
       RotationAnimationData(vec3(90, 0, -177.98), vec3(90, 0, -177.98)),
       getStateDuration(0) + getStateDuration(1)
    );
    animation->addState(firstState, 0);

    AnimationState thirdState(firstState, vec3(-1.84781, 4.73044, 1.43405), vec3(133.332, -5.34221, 209.806), getStateDuration(2));
    animation->addState(thirdState, 0);
    // getStateDuration(3)

    AnimationState fifthState(thirdState, vec3(-1.84781, 4.73044, 1.43405), vec3(146.098, 10.1895, 230.238), getStateDuration(4));
    animation->addState(fifthState, getStateDuration(3));

    AnimationState sixthState(fifthState, vec3(-1.84781, 4.73044, 1.43405), vec3(146.937, 14.281, 246.287), getStateDuration(5));
    animation->addState(sixthState, 0);

    AnimationState seventhState(sixthState, vec3(-1.84781, 4.73044, 1.43405), vec3(146.098, 10.1895, 230.238), getStateDuration(6));
    animation->addState(seventhState, 0);

    AnimationState eighthState(seventhState, vec3(-1.84781, 4.73044, 1.43405), vec3(146.937, 14.281, 246.287), getStateDuration(7));
    animation->addState(eighthState, 0);

    return animation;
}
Animation * createThirdRightLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    AnimationState firstState(
       TranslationAnimationData(vec3(-1.62169, 7.0024, 1.43405), vec3(-1.62167, 7.00705, 1.4409)),
       RotationAnimationData(vec3(90, 0, 168.95), vec3(70.051, -4.05779, 169.785)), //TODO: check difference with left strange
       getStateDuration(0)
    );
    animation->addState(firstState, 0);
    // getStateDuration(1)

    // Finally use 2 time frame for this
    AnimationState fourthState(firstState, vec3(-1.62158, 6.98846, 1.44672), vec3(40, 2.23032, 191.823), getStateDuration(2) + getStateDuration(3));
    animation->addState(fourthState, getStateDuration(1));
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)

    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(-1.62158, 6.98846, 1.44672), vec3(40, 2.23032, 191.823), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));

    return animation;
}
Animation * createFourthRightLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    AnimationState firstState(
       TranslationAnimationData(vec3(-1.48775, 9.08076, 1.50708), vec3(-1.48775, 9.07475, 1.51079)),
       RotationAnimationData(vec3(90, 0, 159.3), vec3(72.0188, -6.89786, 160.507)),
       getStateDuration(0)
    );
    animation->addState(firstState, 0);

    AnimationState secondState(firstState, vec3(-1.48767, 9.07265, 1.51151), vec3(66.3006, -8.94289, 161.331), getStateDuration(1));
    animation->addState(secondState, 0);

    // AnimationState thirdState(secondState, vec3(-1.48763, 9.06925, 1.5122), vec3(57.2258, -11.9482, 163.036), getStateDuration(2));
    // animation->addState(thirdState, 0);

    AnimationState fourthState(secondState, vec3(-1.48754, 9.06249, 1.50708), vec3(50, -35.0532, 151.3), getStateDuration(2) + getStateDuration(3));
    animation->addState(fourthState, 0);
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)

    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(-1.48754, 9.06249, 1.50708), vec3(50, -35.0532, 151.3), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));

    return animation;
}

Animation * getRightLegAnimation(int i) {
    switch (i) {
        case 1:
            return createFirstRightLegAnimation();
        case 2:
            return createSecondRightLegAnimation();
        case 3:
            return createThirdRightLegAnimation();
        case 4:
            return createFourthRightLegAnimation();
        default:
            return NULL;
    }
};

        // Middle Leg
Animation * createThirdRightMiddleLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    // Useless State (no implementation of initial position)
    // state 1 + 2
    AnimationState firstState(
        TranslationAnimationData(vec3(8.70222, 2.23882, -0.35562), vec3(8.70222, 2.23882, -0.35562)),
        RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
        getStateDuration(0) + getStateDuration(1)
    );
    animation->addState(firstState, 0);

    AnimationState thirdState(firstState, vec3(8.70222, 2.23882, -0.35562), vec3(0.892509, 0.893625, -4.4), getStateDuration(2));
    animation->addState(thirdState, 0);

    AnimationState fourthState(thirdState, vec3(8.70222, 2.23882, -0.35562), vec3(12.2, -12.4629, 1.172), getStateDuration(3));
    animation->addState(fourthState, 0);
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)

    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(8.70222, 2.23882, -0.35562), vec3(12.2, -12.4629, 1.172), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));

    return animation;
}
Animation * createFourthRightMiddleLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    // Useless State (no implementation of initial position)
    AnimationState firstState(
        TranslationAnimationData(vec3(8.70222, 2.23882, -0.35562), vec3(8.70222, 2.23882, -0.35562)),
        RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
        getStateDuration(0)
    );
    animation->addState(firstState, 0);

    AnimationState secondState(firstState, vec3(8.69537, 2.2672, -0.373516), vec3(12.892, -0.649946, 5.023), getStateDuration(1));
    animation->addState(secondState, 0);

    // getStateDuration(2)
    
    AnimationState fourthState(secondState, vec3(8.69537, 2.2672, -0.373516), vec3(-2.37984, -1.60723, 5.67), getStateDuration(3));
    animation->addState(fourthState, getStateDuration(2));
    
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)

    // Useless State (juste to allow Loopback later ?)
    AnimationState eighthState(fourthState, vec3(8.69537, 2.2672, -0.373516), vec3(-2.37984, -1.60723, 5.67), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));

    return animation;
}

Animation * getRightMiddleLegAnimation(int i) {
    switch (i) {
        case 3:
            return createThirdRightMiddleLegAnimation();
        case 4:
            return createFourthRightMiddleLegAnimation();
        default:
            return NULL;
    }
};

        // Down Leg
Animation * createThirdRightDownLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    // Useless State (no implementation of initial position)
    AnimationState firstState(
        TranslationAnimationData(vec3(5.06459, -2.83332, -0.526954), vec3(5.06459, -2.83332, -0.526954)),
        RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
        getStateDuration(0)
    );
    animation->addState(firstState, 0);

    AnimationState secondState(firstState, vec3(5.06459, -2.83332, -0.526954), vec3(2.44578, 1.02513, 14.0442), getStateDuration(1));
    animation->addState(secondState, 0);
    
    // getStateDuration(2)

    AnimationState fourthState(secondState, vec3(5.06459, -2.83332, -0.526954), vec3(10.5309, -2.07191, -6.90526), getStateDuration(3));
    animation->addState(fourthState, getStateDuration(2));
    // getStateDuration(4)
    // getStateDuration(5)
    // getStateDuration(6)

    // Useless State (juste to allow Loopback later ?
    AnimationState eighthState(fourthState, vec3(5.06459, -2.83332, -0.526954), vec3(10.5309, -2.07191, -6.90526), getStateDuration(7));
    animation->addState(eighthState, getStateDuration(4) + getStateDuration(5) + getStateDuration(6));

    return animation;
}

Animation * getRightDownLegAnimation(int i) {
    switch (i) {
        case 3:
            return createThirdRightDownLegAnimation();
        default:
            return NULL;
    }
};


// Assembly

HierarchicalModel getLeftLegMiddleModel(int legIndex) {
    // Middle Leg
    Transformation legMiddleTransform;
    legMiddleTransform.translate(vec3(8.70222, 2.23882, 0.35562));
    HierarchicalModel legMiddle(spiderLeftMiddleLeg, legMiddleTransform);
    legMiddle.animation = getLeftMiddleLegAnimation(legIndex);
        
    // Down Leg
    Transformation legDownTransform;
    legDownTransform.translate(vec3(5.06459, -2.8407, 0.526954));
    HierarchicalModel legDown(spiderLeftDownLeg, legDownTransform);
    legDown.animation = getLeftDownLegAnimation(legIndex);

    // Assembly
    legMiddle.addChild(legDown);
    
    return legMiddle;
}
HierarchicalModel getLeftLeg(int legIndex) {
    HierarchicalModel leftLeg(spiderLeftUpLeg, getLeftLegAnimation(legIndex));
    HierarchicalModel leftLegMiddle = getLeftLegMiddleModel(legIndex);
    
    leftLeg.addChild(leftLegMiddle);
    return leftLeg;
}

HierarchicalModel getRightLegMiddleModel(int legIndex) {
    // Middle Leg
    Transformation legMiddleTransform;
    legMiddleTransform.translate(vec3(8.70222, 2.23882, -0.35562));
    HierarchicalModel legMiddle(spiderRightMiddleLeg, legMiddleTransform);
    legMiddle.animation = getRightMiddleLegAnimation(legIndex);
        
    // Down Leg
    Transformation legDownTransform;
    legDownTransform.translate(vec3(5.06459, -2.83332, -0.526954));
    HierarchicalModel legDown(spiderRightDownLeg, legDownTransform);
    legDown.animation = getRightDownLegAnimation(legIndex);

    // Assembly
    legMiddle.addChild(legDown);
    
    return legMiddle;
}
HierarchicalModel getRightLeg(int legIndex) {
    HierarchicalModel rightLeg(spiderRightUpLeg, getRightLegAnimation(legIndex));
    HierarchicalModel rightLegMiddle = getRightLegMiddleModel(legIndex);
    
    rightLeg.addChild(rightLegMiddle);
    return rightLeg;
}

HierarchicalModel createSpiderModel(Transformation baseTransform, int frameBySecond) {
    initModels();
    
    FRAME_BY_SECOND = frameBySecond;
    spiderQty++;
    ANIMATION_SYNCHRO_STRING = "spider_" + to_string(spiderQty);
    
    HierarchicalModel body(spiderBody, createBodyAnimation(), baseTransform);

    // Left Legs
    for (int i = 1; i <= 4; i++) {
        HierarchicalModel leftLeg = getLeftLeg(i);
        body.addChild(leftLeg);
    }
    
    // Right Legs
    for (int i = 1; i <= 4; i++) {
        HierarchicalModel rightLeg = getRightLeg(i);
        body.addChild(rightLeg);
    }
    
    return body;
}


/* WORKED ANIMATION !ABOVE! */
/* DEMO ANIMATION PART */
/* ANIMATION TO SHOWCASE HIEARCHY !UNDER! */


Animation * createDemoBodyAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);
    
    AnimationState firstState(
       TranslationAnimationData(vec3(0, 0, 5.48044), vec3(0, -7.22033, 5.48044)),
       RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
       frameToSecond(5)
    );
    animation->addState(firstState, frameToSecond(0));
    
    AnimationState secondState(firstState, vec3(0, -7.22033, 5.48044), vec3(0, 0, 0), frameToSecond(5));
    animation->addState(secondState, frameToSecond(10));
    
    return animation;
}

Animation * createDemoLeftLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    AnimationState firstState(
       TranslationAnimationData(vec3(0.933725, 2.47272, 1.38035), vec3(0.933725, 2.47272, 1.38035)),
       RotationAnimationData(vec3(90, 0, -17), vec3(90, 0, -17)),
       frameToSecond(5)
    );
    animation->addState(firstState, frameToSecond(0));

    AnimationState secondState(firstState, vec3(0.933725, 2.47272, 1.38035), vec3(111.88, -50.6345, -27.4), frameToSecond(5));
    animation->addState(secondState, frameToSecond(0));
    
    AnimationState thirdState(secondState, vec3(0.933725, 2.47272, 1.38035), vec3(111.88, -50.6345, -27.4), frameToSecond(5));
    animation->addState(thirdState, frameToSecond(5));
    
    return animation;
}
Animation * createDemoLeftMiddleLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    AnimationState state(
       TranslationAnimationData(vec3(8.70222, 2.23882, 0.35562), vec3(8.70222, 2.23882, 0.35562)),
       RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
       frameToSecond(5)
    );
    animation->addState(state, frameToSecond(0));
    
    AnimationState secondState(state, vec3(8.70222, 2.23882, 0.35562), vec3(-26.6394, -31.0399, 29.8854), frameToSecond(5));
    animation->addState(secondState, frameToSecond(5));
    
    AnimationState thirdState(secondState, vec3(8.70222, 2.23882, 0.35562), vec3(-26.6394, -31.0399, 29.8854), frameToSecond(5));
    animation->addState(thirdState, frameToSecond(0));
    
    return animation;
}

Animation * createDemoLeftDownLegAnimation() {
    Animation * animation = new Animation(ANIMATION_SYNCHRO_STRING);

    AnimationState state(
       TranslationAnimationData(vec3(5.06459, -2.8407, 0.526954), vec3(5.06459, -2.8407, 0.526954)),
       RotationAnimationData(vec3(0, 0, 0), vec3(0, 0, 0)),
       frameToSecond(10)
    );
    animation->addState(state, frameToSecond(0));
    
    AnimationState secondState(state, vec3(5.06459, -2.8407, 0.526954), vec3(-39.3161, -14.8814, -56.6991), frameToSecond(5));
    animation->addState(secondState, frameToSecond(5));
    
    return animation;
}

HierarchicalModel getDemoLeftLegMiddleModel(int legIndex) {
    // Middle Leg
    Transformation legMiddleTransform;
    legMiddleTransform.translate(vec3(8.70222, 2.23882, 0.35562));
    HierarchicalModel legMiddle(spiderLeftMiddleLeg, legMiddleTransform);
    if (legIndex == 0) {
        legMiddle.animation = createDemoLeftMiddleLegAnimation();
    }
        
    // Down Leg
    Transformation legDownTransform;
    legDownTransform.translate(vec3(5.06459, -2.8407, 0.526954));
    HierarchicalModel legDown(spiderLeftDownLeg, legDownTransform);
    if (legIndex == 0) {
        legDown.animation = createDemoLeftDownLegAnimation();
    }
    
    // Assembly
    legMiddle.addChild(legDown);
    
    return legMiddle;
}
HierarchicalModel getDemoLeftLeg(int legIndex) {
    HierarchicalModel leftLeg(spiderLeftUpLeg, leftLegsTransform[legIndex]);
    HierarchicalModel leftLegMiddle = getDemoLeftLegMiddleModel(legIndex);
    
    leftLeg.addChild(leftLegMiddle);
    return leftLeg;
}

HierarchicalModel getDemoRightLegMiddleModel(int legIndex) {
    // Middle Leg
    Transformation legMiddleTransform;
    legMiddleTransform.translate(vec3(8.70222, 2.23882, -0.35562));
    HierarchicalModel legMiddle(spiderRightMiddleLeg, legMiddleTransform);
        
    // Down Leg
    Transformation legDownTransform;
    legDownTransform.translate(vec3(5.06459, -2.83332, -0.526954));
    HierarchicalModel legDown(spiderRightDownLeg, legDownTransform);

    // Assembly
    legMiddle.addChild(legDown);
    
    return legMiddle;
}
HierarchicalModel getDemoRightLeg(int legIndex) {
    HierarchicalModel rightLeg(spiderRightUpLeg, rightLegsTransform[legIndex]);
    HierarchicalModel rightLegMiddle = getDemoRightLegMiddleModel(legIndex);
    
    rightLeg.addChild(rightLegMiddle);
    return rightLeg;
}

HierarchicalModel createDemoSpiderModel(Transformation baseTransform, int frameBySecond) {
    initModels();
    
    FRAME_BY_SECOND = frameBySecond;
    ANIMATION_SYNCHRO_STRING = "spider_demo";
    
    HierarchicalModel body(spiderBody, createDemoBodyAnimation(), baseTransform);

    // Left Legs
    for (int i = 0; i < 4; i++) {
        HierarchicalModel leftLeg = getDemoLeftLeg(i);
        if (i == 0) {
            leftLeg.animation = createDemoLeftLegAnimation();
        }
        body.addChild(leftLeg);
    }
    
    // Right Legs
    for (int i = 0; i < 4; i++) {
        HierarchicalModel rightLeg = getDemoRightLeg(i);

        body.addChild(rightLeg);
    }
    
    return body;
}
