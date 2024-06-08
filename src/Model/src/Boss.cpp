//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Boss.h"


Boss::Boss(Position3D position){
    this->position = position;
    speed = 1;
    type = BOSS;
    hp = 10;
    dead = false;
}


void Boss::shoot(Position3D inputPosition, const float deltaT)
{
    Position3D shootingVector;
    shootingVector.origin = position.origin;
    shootingVector.orientation = (position.origin - inputPosition.origin);
    if(checkDistance3D(inputPosition.origin, position.origin, BOSS))
        bullets->insert(new Bullet(shootingVector, BOSS, false));
}

void Boss::bossMovement(Position3D playerPosition, float deltaT)
{
    if(!checkDistance3D(playerPosition.origin, position.origin, BOSS))
        moveTowardsPoint(playerPosition, deltaT);
    else
    {
        circularMovement(playerPosition, deltaT);
    }
}

void Boss::circularMovement(Position3D center, float deltaT)
{
    float radius = 10.0f;
    position.origin.x += center.origin.x + radius*glm::cos(deltaT);
    position.origin.z += center.origin.z + radius*glm::sin(deltaT);
    changeDirection(center, deltaT);
}



