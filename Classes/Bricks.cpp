#include "Bricks.h"
#include "cocos2d.h"
using namespace cocos2d;

#define SQRT_3 1.732050808

namespace bricks {

Node *new_circle(float radius, PhysicsMaterial material)
{
    Node *node = Node::create();
    node->setContentSize(Size(radius * 2, radius * 2));
    // Draw two circles to make a ring
    // DrawNode's drawDot is not anti-aliased...
    auto s = Sprite::create("images/circle.png");
    s->setPosition(radius, radius);
    s->setScale(radius * 2 / s->getContentSize().width);
    s->setColor(Color3B::BLACK);
    node->addChild(s, 1);
    s = Sprite::create("images/circle.png");
    s->setPosition(radius, radius);
    s->setScale((radius - 2) * 2 / s->getContentSize().width);
    s->setColor(Color3B(255, 192, 192));
    node->addChild(s, 2, TAG_COLOURED_PART);
    PhysicsBody *body = PhysicsBody::createCircle(radius, material);
    // Stop those warnings
    // cocos2d: Node warning: setPhysicsBody sets anchor point to Vec2::ANCHOR_MIDDLE.
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPhysicsBody(body);
    return node;
}

Node *new_triangle(float sidelen, PhysicsMaterial material)
{
    float radius = sidelen / SQRT_3;
    Node *node = Node::create();
    node->setContentSize(Size(sidelen, radius * 1.5));
    auto s = Sprite::create("images/triangle.png");
    s->setPosition(Vec2(sidelen, radius * 1.5));
    s->setScale(sidelen / s->getContentSize().width);
    s->setColor(Color3B::BLACK);
    node->addChild(s, 1);
    s = Sprite::create("images/triangle.png");
    s->setPosition(Vec2(sidelen, radius * 1.5 - 0.5));
    s->setScale((sidelen - 4.0 * SQRT_3) / s->getContentSize().width);
    s->setColor(Color3B(128, 255, 128));
    node->addChild(s, 2, TAG_COLOURED_PART);
    Vec2 points[] = { Vec2(sidelen * 0.5, radius * 1.5), Vec2(sidelen, 0), Vec2(0, 0) };
    PhysicsBody *body = PhysicsBody::createPolygon(points, 3, material);
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPhysicsBody(body);
    return node;
}

Node *new_rect(float width, float height, PhysicsMaterial material)
{
    Node *node = Node::create();
    node->setContentSize(Size(width, height));
    // Now that segments don't need anti-aliasing, why don't use DrawNode?
    // Well, DrawNode doesn't support auto-batching...
    // T^T  <(Why me?!!!)  <-- DrawNode
    auto s = Sprite::create("images/white_pixel.png");
    s->setPosition(Vec2(width * 0.5, height * 0.5));
    s->setScale(width, height);
    s->setColor(Color3B::BLACK);
    node->addChild(s, 1);
    s = Sprite::create("images/white_pixel.png");
    s->setPosition(Vec2(width * 0.5, height * 0.5));
    s->setScale(width - 4, height - 4);
    s->setColor(Color3B(128, 128, 255));
    s->getTexture()->setAntiAliasTexParameters();
    node->addChild(s, 2, TAG_COLOURED_PART);
    PhysicsBody *body = PhysicsBody::createBox(Size(width, height), material);
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPhysicsBody(body);
    return node;
}

Node *new_random(float radius, etype enabled_types, PhysicsMaterial material)
{
    if (enabled_types & 7 == 0) enabled_types = FLAG_ALL_ENABLED;
    while (true) {
        int r = rand() % 3;
        switch (r) {
        case 0:
            if (enabled_types & (1<<0))
                return new_circle(radius, material);
            break;
        case 1:
            if (enabled_types & (1<<1))
                return new_rect(radius * 2, radius * 2, material);
            break;
        case 2:
            if (enabled_types & (1<<2))
                return new_triangle(radius * SQRT_3, material);
            break;
        default:
            // Never reach this!
            return nullptr;
        }
    }
}

Node *get_coloured_part(Node *brick)
{
    return brick->getChildByTag(TAG_COLOURED_PART);
}

void set_brick_colour(Node *brick, Color3B colour)
{
    Node *coloured_part = get_coloured_part(brick);
    if (coloured_part) coloured_part->setColor(colour);
}

}

