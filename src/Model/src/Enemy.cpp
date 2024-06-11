//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Enemy.h"


Enemy::Enemy(Position3D position) {

    this->position = position;
    speed = 1;
    type = ENEMY;
    hp = 1;
    dead = false;
}

/*void Enemy::changePosition(Position3D inputPosition, const float deltaT)
{
    float x =  glm::sin(glm::radians(position.orientation.y)) * speed * deltaT;
    float y =  0.0f;
    float z =  glm::cos(glm::radians(position.orientation.y)) * speed * deltaT;
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
    position.origin= position.origin * T;
}

void Enemy::changeDirection(Position3D inputPosition, const float deltaT)
{
    glm::vec3 pointingDirection = glm::vec4(glm::sin(glm::radians(position.orientation.y)), 0.0f, glm::cos(glm::radians(position.orientation.y)), 1.0f);
    glm::vec3 cross  = glm::cross(pointingDirection, glm::vec3(inputPosition.origin));
    if(cross.y > 0)
        position.orientation.y -= speed * deltaT;
    else if (cross.y < 0)
        position.orientation.y += speed * deltaT;
    //TODO If cross==0
}*/

/**
 * Enemy will shoot towards Player if he is sufficiently close and if his direction vector intersect a sphere centered
 * in the Player's position.
 * @param inputPosition the Player's position
 * @param deltaT time
 */
void Enemy::shoot(Position3D inputPosition, const float deltaT)
{
    float distance;
    if(glm::intersectRaySphere(position.origin, position.orientation, inputPosition.origin, pow(1.0f, 2.0f), distance)
        && checkDistance3D(inputPosition.origin, position.origin, ENEMY))
        bullets->insert(new Bullet(position, ENEMY, false));
}



/*void Enemy::shoot(const float deltaT,float SCREEN_WIDTH, float SCREEN_HEIGHT)
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
/*void Enemy::move(const Player& player,const float deltaT)
{
    const glm::vec3& direction = glm::normalize(this->position3D_.origin);
    position3D_.origin += direction * speed_ * deltaT;
    if (debug_)
    {
        std::lock_guard<std::mutex> lock(projectiles_mutex_);
        std::cout<< "Enemy position is:"<<glm::to_string(position3D_.origin)<<std::endl;
    }
}

/*void Enemy::moveProjectile(unsigned int projectileId, float deltaT,
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
}*/
