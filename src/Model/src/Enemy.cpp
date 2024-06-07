//
// Created by feder34 on 21/05/24.
//

#include "../Include/Enemy.h"

Enemy::Enemy(const Position3D& position3D,const float speed, bool debug):
        position3D_(position3D),speed_(speed),debug_(debug){};

void Enemy::eraseProjectile(unsigned  int id)
{
    projectiles_.erase(id);
    if(debug_)
        std::cout<<"I AM erasing projectile id:"<<id<<std::endl;
}
const Position3D& Enemy::getPosition() const
{
    return position3D_;
}
void Enemy::shoot(const float deltaT,float SCREEN_WIDTH, float SCREEN_HEIGHT)
{
    Bullet bullet(position3D_, 50.0f, 3.0f, true); // velocita del proiettile da cambiare
    projectiles_.insert({bullet.getId(),bullet});

    std::thread(&Enemy::moveProjectile, this, bullet.getId(),deltaT,
                SCREEN_WIDTH, SCREEN_HEIGHT).detach();
    if (debug_)
    {
        std::lock_guard<std::mutex> lock(projectiles_mutex_);
            std::cout << "Enemy in position:" << glm::to_string(position3D_.origin)
                      << "is shooting bullet:" << bullet.getId() << std::endl;
    }
}
void Enemy::move(const Player& player,const float deltaT)
{
    const glm::vec3& direction = glm::normalize(player.origin_ - this->position3D_.origin);
    position3D_.origin += direction * speed_ * deltaT;
    if (debug_)
    {
        std::lock_guard<std::mutex> lock(projectiles_mutex_);
        std::cout<< "Enemy position is:"<<glm::to_string(position3D_.origin)<<std::endl;
    }
}

void Enemy::moveProjectile(unsigned int projectileId, float deltaT,
                           float SCREEN_WIDTH, float SCREEN_HEIGHT)
{
    while (true) {
        {
            std::lock_guard<std::mutex> lock(projectiles_mutex_);
            auto it = projectiles_.find(projectileId);
            it->second.move(deltaT);
            if(!it->second.isPositionInsideScreen(SCREEN_WIDTH, SCREEN_HEIGHT)) {
                projectiles_.erase(it);
                std::cout << "Bullet " << projectileId << " is out of bounds and removed.\n";
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Move every 50 milliseconds
    }
}
