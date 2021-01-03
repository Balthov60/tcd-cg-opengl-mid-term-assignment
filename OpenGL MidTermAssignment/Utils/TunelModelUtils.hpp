//
//  TunelModelUtils.h
//  OpenGL MidTermAssignment
//
//  Created by Balthazar Frolin on 02/01/2021.
//

#pragma once
#include "CGUtils.hpp"
#include "Transformation.hpp"
#include "HierarchicalModel.hpp"

HierarchicalModel createTunelModel(Transformation baseTransform)
{
    /* prepare */
    
    Model * tunelFloorModel = new Model("tunel/tunel_floor.dae");
    Model * tunelTopModel = new Model("tunel/tunel_top.dae");
    
    Texture * floorTexture = new Texture("goudron_color.jpg");
    Texture * roofTexture = new Texture("brique.jpg");
    
    tunelTopModel->texture = roofTexture;
    tunelFloorModel->texture = floorTexture;

    HierarchicalModel tunelTop(tunelTopModel, baseTransform);

    /* assembly */
    
    HierarchicalModel tunel(tunelFloorModel, baseTransform);
    tunel.addChild(tunelTop);
    baseTransform.translate(vec3(0, 0, -60));
    
    HierarchicalModel tunel2(tunelFloorModel, baseTransform);
    tunel2.addChild(tunelTop);
    baseTransform.translate(vec3(0, 0, 120));
    
    HierarchicalModel tunel3(tunelFloorModel, baseTransform);
    tunel3.addChild(tunelTop);
    
    tunel.addChild(tunel3);
    tunel.addChild(tunel2);

    return tunel;
}
