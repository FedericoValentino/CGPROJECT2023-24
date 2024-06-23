//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Boss.h"


Boss::Boss(Position3D position){
    this->position = position;
    translationSpeed = 1;
    rotationSpeed = glm::radians(120.0f);
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
void Boss::shoot(Position3D inputPosition, const float deltaT)
{
    Position3D shootingVector;
    shootingVector.origin = position.origin;
    shootingVector.rotation = (position.origin - inputPosition.origin);
    if(checkDistance3D(inputPosition.origin, position.origin, BOSS))
        bullets->insert(new Bullet(shootingVector, BOSS, false));
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
    if(!checkDistance3D(playerPosition.origin, position.origin, BOSS))
        moveTowardsPoint(playerPosition, deltaT);
    else
    {
        circularMovement(playerPosition, deltaT);
    }
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
    position.origin.x += center.origin.x + radius*glm::cos(deltaT);
    position.origin.z += center.origin.z + radius*glm::sin(deltaT);
    changeDirection(center, deltaT);
}



