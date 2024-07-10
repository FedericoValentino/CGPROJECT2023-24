//
// Created by feder34 on 21/05/24.
//

#include "../Include/Bullet.h"
#include "glm/glm.hpp"


unsigned int Bullet::count_ = 0;

/**
 * Bullet Constructor gives different characteristics to each bullet based on the entity spawning it.
 * @param position3D the position of the entity generating the bullet
 * @param type determines whether the entity generating the bullet is a Player, an Enemy or a Boss.
 * @param debug
 */
Bullet::Bullet(const Position3D& position3D, PLANETYPE_UTILITY type, bool debug)
{
    toClear = false;
    this->position3D_=position3D;
    debug = false;
    id_ = count_;
    count_++;
    this->owner = type;
    switch (type){
        case(PLAYER):
            speed_ = 30.0f;
            size_ = 5.0f;
            break;
        case(ENEMY):
            speed_ = 15.0f;
            size_ = 3.0f;
            break;
        case(BOSS):
            speed_ = 20.0f;
            size_ = 10.0f;
            break;
        default:
            break;
    }
}


const Position3D& Bullet::getPosition3D() const {
    return position3D_;
}

float Bullet::getSize() const {
    return size_;
}

float Bullet::getSpeed() const {
    return speed_;
}

unsigned int Bullet::getId() const
{
    return id_;
}

void Bullet::move(const float deltaT) {
    auto direction = glm::vec3(glm::sin(position3D_.rotation.y), 0.0f, glm::cos(position3D_.rotation.y));
    position3D_.origin += direction * speed_ * deltaT;
}

bool Bullet::isPositionInsideScreen(const float SCREEN_WIDTH,
                                    const float SCREEN_HEIGHT) const
{
    return (position3D_.origin.x >= 0 && position3D_.origin.x < SCREEN_WIDTH &&
            position3D_.origin.y >= 0 && position3D_.origin.y < SCREEN_HEIGHT);
}

PLANETYPE_UTILITY Bullet::getType() {
    return owner;
}
