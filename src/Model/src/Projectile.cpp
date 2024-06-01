//
// Created by feder34 on 21/05/24.
//

#include "../Include/Projectile.h"
#include "glm/glm.hpp"


unsigned int Projectile::count_ = 0;

Projectile::Projectile(const Position3D& position3D, float size, float speed ,bool debug):
        position3D_(position3D),size_(size),speed_(speed),id_(++count_),debug_(debug){};

const Position3D& Projectile::getPosition3D() const {
    return position3D_;
}

float Projectile::getSize() const {
    return size_;
}

float Projectile::getSpeed() const {
    return speed_;
}

unsigned int Projectile::getId() const
{
    return id_;
}

void Projectile::move(const float deltaT) {
    position3D_.origin += position3D_.orientation * speed_ * deltaT;
    if(debug_)
    {
        std::cout<<"I AM BULLET:"<<id_<<std::endl;
        std::cout<<"origin:"<<glm::to_string(position3D_.origin)<<std::endl;
        std::cout<<"orientation:"<<glm::to_string(position3D_.orientation)<<std::endl;
    }

}

bool Projectile::isPositionInsideScreen(const float SCREEN_WIDTH,
                                        const float SCREEN_HEIGHT) const
{
    return (position3D_.origin.x >= 0 && position3D_.origin.x < SCREEN_WIDTH &&
            position3D_.origin.y >= 0 && position3D_.origin.y < SCREEN_HEIGHT);
}
