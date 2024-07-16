//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Enemy.h"


Enemy::Enemy(const Position3D& position) {
    this->position = position;
    translationSpeed = 8;
    rotationSpeed = glm::radians(60.0f);
    type = ENEMY;
    hp = 1;
    dead = false;
    evasionTimer = 0.0;
}


/**
 * Enemy will shoot towards Player if he is sufficiently close and if his direction vector intersect a sphere centered
 * in the Player's position.
 * @param inputPosition the Player's position
 * @param deltaT time
 */
std::shared_ptr<Bullet> Enemy::shoot(const Position3D& inputPosition, const float deltaT)
{
    float distance;
    std::shared_ptr<Bullet> b = nullptr;
    glm::vec3 dir = glm::vec3(glm::sin(position.rotation.y), 0.0f, glm::cos(position.rotation.y));
    if(glm::intersectRaySphere(position.origin, dir, inputPosition.origin, pow(1.0f, 2.0f), distance)
        && checkDistance3D(inputPosition.origin, position.origin, ENEMY)
        && (elapsedTime > 2.0f || bullets->empty()))
    {
        b = std::make_shared<Bullet>(position,ENEMY,false);
        bullets->insert(b);
        elapsedTime = 0;
    }
    return b;
}

