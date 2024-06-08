//
// Created by feder34 on 21/05/24.
//

#include "../Include/Bullet.h"
#include "glm/glm.hpp"


unsigned int Bullet::count_ = 0;

Bullet::Bullet(const Position3D& position3D, PLANETYPE type, bool debug)
{
    this->position3D_=position3D;
    debug = false;
    id_ = count_;
    count_++;
    switch (type){
        case(PLAYER):
            speed_ = 2.0f;
            size_ = 1.0f;
            break;
        case(ENEMY):
            speed_ = 1.0f;
            size_ = 1.0f;
            break;
        case(BOSS):
            speed_ = 3.0f;
            size_ = 3.0f;
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
    position3D_.origin += position3D_.orientation * speed_ * deltaT;
    if(debug_)
    {
        std::cout<<"I AM BULLET:"<<id_<<std::endl;
        std::cout<<"origin:"<<glm::to_string(position3D_.origin)<<std::endl;
        std::cout<<"orientation:"<<glm::to_string(position3D_.orientation)<<std::endl;
    }

}

bool Bullet::isPositionInsideScreen(const float SCREEN_WIDTH,
                                    const float SCREEN_HEIGHT) const
{
    return (position3D_.origin.x >= 0 && position3D_.origin.x < SCREEN_WIDTH &&
            position3D_.origin.y >= 0 && position3D_.origin.y < SCREEN_HEIGHT);
}
