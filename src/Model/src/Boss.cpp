//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Boss.h"
#include <glm/gtx/vector_angle.hpp>


Boss::Boss(const Position3D& position){
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
std::shared_ptr<Bullet> Boss::shoot(const Position3D& inputPosition, const float deltaT)
{
    std::shared_ptr<Bullet> bullet = nullptr;
    if((!avoidBuilding || position.origin.y >= 11.0f) || (avoidBuilding && position.origin.y >= 11.0f)) {
        Position3D shootingVector;
        shootingVector.origin = position.origin;

        shootingVector.rotation = glm::normalize((inputPosition.origin - position.origin));
        shootingVector.rotation = glm::vec3(atan2(shootingVector.rotation.y, shootingVector.rotation.z),
                                            atan2(shootingVector.rotation.x, shootingVector.rotation.z),
                                            0.0f);
        if (checkDistance3D(inputPosition.origin, position.origin, BOSS)
            && (elapsedTime > 1.0f || (bullets->empty() && elapsedTime > 1.0f))) {
            bullet = std::make_shared<Bullet>(shootingVector, BOSS, false);
            bullets->insert(bullet);
            elapsedTime = 0;
        }
    }
    return  bullet;
}


/**
 * Disciplines how the BOSS should move given the circumstances; if BOSS is too far from the Player, it will fly
 * towards the Player's position like a normal enemy. Otherwise, it will stop to shoot at them
 * @param playerPosition the Player's position
 * @param deltaT time
 */
void Boss::bossMovement(const Position3D& playerPosition, float deltaT)
{
    Position3D tempPosForward{};
    tempPosForward.origin = glm::vec3(playerPosition.origin.x + glm::sin(playerPosition.rotation.y) * 10.0f,
                                      playerPosition.origin.y,
                                      playerPosition.origin.z + glm::cos(playerPosition.rotation.y) * 10.0f);

    Position3D tempPosBackward{};
    tempPosBackward.origin = glm::vec3(playerPosition.origin.x - glm::sin(playerPosition.rotation.y) * 10.0f,
                                      playerPosition.origin.y,
                                      playerPosition.origin.z - glm::cos(playerPosition.rotation.y) * 10.0f);


    verticalMovement(deltaT);
    avoidBuilding = false;
    // if boss is far from player
    if(!checkDistance3D(playerPosition.origin, position.origin, BOSS))
    {
        if (glm::distance(position.origin, tempPosBackward.origin) >
            glm::distance(position.origin, tempPosForward.origin))
            moveTowardsPoint(tempPosForward, deltaT);
        else
            moveTowardsPoint(tempPosBackward, deltaT);
    }
}


void Boss::changeDirection(const Position3D& playerPosition, float deltaT)
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


void Boss::verticalMovement(float deltaT)
{
    if(avoidBuilding)
    {
        if (position.origin.y < 15.0f)
            position.origin.y += translationSpeed/5.0f * deltaT;
    }
    else
    {
        if (position.origin.y > 8.40f)
            position.origin.y -= translationSpeed/5.0f * deltaT;
    }
}





