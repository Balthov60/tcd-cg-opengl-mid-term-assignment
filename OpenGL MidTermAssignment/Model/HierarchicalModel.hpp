//
//  HierarchicalModel.h
//  TriangleDemo
//
//  Created by Balthazar Frolin on 10/11/2020.
//

#pragma once
#include "CGUtils.hpp"
#include "Model.hpp"
#include "Animation.hpp"

class HierarchicalModel
{
public:
    vector<HierarchicalModel> childs;
    Animation * animation = NULL;
    
    HierarchicalModel(Model * model, Transformation transform): baseTransform(transform) {
        this->model = model;
    }
    HierarchicalModel(Model * model, Animation * animation, Transformation additionalTransform = Transformation()) {
        this->model = model;
        this->animation = animation;
        this->additionnalTransform = additionalTransform;
    }
    
    void draw(ShaderProgram shaderProgram, Transformation parentTransform) {
        Transformation transform = getBaseTransform(parentTransform);
        model->draw(shaderProgram, transform);
        
        for (HierarchicalModel &child : childs) {
            child.draw(shaderProgram, transform);
        }
    }
    
    void addChild(HierarchicalModel child) {
        childs.push_back(child);
    }
    
private:
    Model * model;
    
    
    // base transform ignored if animation used
    Transformation baseTransform;
    
    // additionnal transform to use on animation
    Transformation additionnalTransform;
    
    /**
     Return base tranform for this object based on animation if present or on base transform variable in the other case
     */
    Transformation getBaseTransform(Transformation parentTransform)
    {
        if (animation != NULL) {
            return parentTransform * additionnalTransform * animation->updateAndGetTransform();
        }
        else {
            return parentTransform * baseTransform;
        }
    }
};
