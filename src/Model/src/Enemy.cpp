//
// Created by feder34 on 21/05/24.
//

#include "../Include/Enemy.h"
#include <thread>

Enemy::Enemy(const Position3D& position3D,const float speed, bool debug):
        position3D_(position3D),speed_(speed),debug_(debug){};

void Enemy::eraseProjectile(unsigned  int id)
{
    projectiles_.erase(id);
}
const Position3D& Enemy::getPosition() const
{
    return position3D_;
}
int Enemy::shoot()
{
    Projectile bullet(position3D_,2.0f,3.0f,true);
    projectiles_.insert({bullet.getId(),bullet});
    projectiles_.at(bullet.getId()).move(2.0f);
}
void Enemy::move(const Player& player,const float deltaT)
{
    const glm::vec3& direction = -this->position3D_.origin;
    position3D_.origin += direction * speed_ * deltaT;
}