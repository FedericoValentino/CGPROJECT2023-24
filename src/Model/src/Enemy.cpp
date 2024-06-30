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
