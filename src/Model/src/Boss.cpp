//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Boss.h"
#include <glm/gtx/vector_angle.hpp>


Boss::Boss(Position3D position){
    this->position = position;
    translationSpeed = 10;
    rotationSpeed = glm::radians(45.0f);
    type = BOSS;
    hp = 10;
    dead = false;
}

/**
 * Handles the shooting behaviour of the BOSS. If BOSS position is within a sphere centered in player with a certain radius,
 * decided parametrically in method checkDistance3D, BOSS will shoot towards the player
 * @param inputPosition the Player's position
 * @param deltaT time
 */
Bullet* Boss::shoot(Position3D inputPosition, const float deltaT)
{
    Position3D shootingVector;
    shootingVector.origin = position.origin;
    shootingVector.rotation = glm::normalize((inputPosition.origin - position.origin));
    shootingVector.rotation = glm::vec3(0.0f, atan2(shootingVector.rotation.x, shootingVector.rotation.z), 0.0f);
    Bullet* bullet = nullptr;
    if(checkDistance3D(inputPosition.origin, position.origin, BOSS)
        && (elapsedTime > 1.0f || bullets->empty())) {
        bullet = new Bullet(shootingVector, BOSS, false);
        bullets->insert(bullet);
        elapsedTime = 0;
    }
    return bullet;
}


/**
 * Disciplines how the BOSS should move given the circumstances; if BOSS is too far from the Player, it will fly
 * towards the Player's position like a normal enemy. Otherwise, it will start flying in circle around the
 * Player's position
 * @param playerPosition the Player's position
 * @param deltaT time
 */
void Boss::bossMovement(Position3D playerPosition, float deltaT)
{
    Position3D tempPosForward{};
    tempPosForward.origin = glm::vec3(playerPosition.origin.x + glm::sin(playerPosition.rotation.y) * 10.0f,
                                      playerPosition.origin.y,
                                      playerPosition.origin.z + glm::cos(playerPosition.rotation.y) * 10.0f);

    Position3D tempPosBackward{};
    tempPosBackward.origin = glm::vec3(playerPosition.origin.x - glm::sin(playerPosition.rotation.y) * 10.0f,
                                      playerPosition.origin.y,
                                      playerPosition.origin.z - glm::cos(playerPosition.rotation.y) * 10.0f);


    // if boss is far from playre
    if(!checkDistance3D(playerPosition.origin, position.origin, BOSS))
        if(glm::distance(position.origin, tempPosBackward.origin) > glm::distance(position.origin, tempPosForward.origin))
            moveTowardsPoint(tempPosForward, deltaT);
        else
            moveTowardsPoint(tempPosBackward, deltaT);

}


/**
 * Changes the Boss's position around a circumference centered in the Player's position.
 *  The direction is changed normally, so BOSS will rotate towards the center.
 * @param center the Player's position
 * @param deltaT time
 */
void Boss::circularMovement(Position3D center, float deltaT)
{
    float radius = 10.0f;

    glm::vec3 toPlayer = glm::vec3(glm::sin(position.rotation.y), 8.40f, glm::cos(position.rotation.y));
    glm::vec3 right = glm::normalize(glm::cross(toPlayer, glm::vec3(0.0f, 1.0f, 0.0f)));
    float circlingDirection = (glm::dot(right, toPlayer) > 0.0f) ? 1.0f : -1.0f;

    glm::vec3 offset = position.origin - center.origin;

    float angle = rotationSpeed * deltaT * circlingDirection;
    float sinAngle = glm::sin(angle);
    float cosAngle = glm::cos(angle);

    glm::vec3 newOffset;
    newOffset.x = cosAngle * offset.x - sinAngle * offset.z;
    newOffset.z = sinAngle * offset.x + cosAngle * offset.z;
    newOffset.y = offset.y;

    position.origin = center.origin + glm::normalize(newOffset) * radius;

    Position3D pos{};

    pos.origin.x = center.origin.x + radius*glm::cos(angle * 2);
    pos.origin.y = center.origin.y;
    pos.origin.z = center.origin.z + radius*glm::cos(angle * 2);

    changeDirection(pos, deltaT);
}

void Boss::changeDirection(Position3D playerPosition, float deltaT)
{
    float target_x = playerPosition.origin.x;
    float target_z = playerPosition.origin.z;

    float pos_x = position.origin.x;
    float pos_z = position.origin.z;

    float dir_x = glm::sin(position.rotation.y);
    float dir_z = glm::cos(position.rotation.y);

    if ((pos_x - target_x) * dir_z > (pos_z - target_z) * dir_x)
        position.rotation.y -= rotationSpeed * deltaT;
    else if((pos_x - target_x) * dir_z < (pos_z - target_z) * dir_x)
        position.rotation.y += rotationSpeed * deltaT;

    position.rotation.y = fmod(position.rotation.y, 2.0f * M_PI);

}



