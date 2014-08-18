#ifndef __BRICKS_H__
#define __BRICKS_H__

#include "cocos2d.h"
using namespace cocos2d;

namespace bricks {

// Basics
// Creates a rectangle with a pure colour.
Sprite *purity(float width, float height, Color3B colour);
Sprite *purity(Size size, Color3B colour);
Sprite *purity(float sidelen, Color3B colour);

const int TAG_COLOURED_PART = 0147106;
const int BRICK_TYPE_COUNT = 3;
// http://stackoverflow.com/questions/1231147/is-int-in-c-always-32-bit
// http://stackoverflow.com/questions/1229131/how-to-declare-a-32-bit-integer-in-c
typedef int32_t etype;
const etype FLAG_ALL_ENABLED = 0xffffffff;

// Bricks
Node *new_circle(float radius,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *new_triangle(float sidelen,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *new_rect(float width, float height,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *new_random(float radius,
    etype enabled_types = FLAG_ALL_ENABLED,
    PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT);

Node *get_coloured_part(Node *brick);
void set_brick_colour(Node *brick, Color3B colour);

// Edge sensor lines (bodies that are not affected by gravity)
// set_brick_colour can also be used on this
Node *new_sensorline(Vec2 startpos, Vec2 endpos);

}

#endif

