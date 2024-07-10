//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Enemy.h"


Enemy::Enemy(Position3D position) {

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
Bullet* Enemy::shoot(Position3D inputPosition, const float deltaT)
{
    float distance;
    Bullet* b = nullptr;
    glm::vec3 dir = glm::vec3(glm::sin(position.rotation.y), 0.0f, glm::cos(position.rotation.y));
    if(glm::intersectRaySphere(position.origin, dir, inputPosition.origin, pow(1.0f, 2.0f), distance)
        && checkDistance3D(inputPosition.origin, position.origin, ENEMY)
        && (elapsedTime > 2.0f || bullets->empty()))
    {
        b = new Bullet(position, ENEMY, false);
        bullets->insert(b);
        elapsedTime = 0;
    }
    return b;
}


/*void Plane::roll(int direction, float deltaT) {
    if(position.rotation.z + rotationSpeed/2.0f *  deltaT <= M_PI/4 && position.rotation.z + rotationSpeed/2.0f *  deltaT >= -M_PI/4)
        position.rotation.z += rotationSpeed/2.0f * direction * deltaT; // roll
    if (position.rotation.z < -M_PI/4)
    {
        position.rotation.z = -M_PI/4;
    }
    else if (position.rotation.z > M_PI/4)
    {
        position.rotation.z = M_PI/4;
    }
}*/

