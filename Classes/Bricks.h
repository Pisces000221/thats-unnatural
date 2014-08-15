#ifndef __BRICKS_H__
#define __BRICKS_H__

#include "cocos2d.h"
using namespace cocos2d;

namespace bricks {

Node *new_circle(float radius,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *new_triangle(float sidelen,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *new_rect(float width, float height,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *new_random(float radius,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

}

#endif

