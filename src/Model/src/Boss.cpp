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

void Boss::changePosition(Position3D inputPosition, const float deltaT)
{

}

void Boss::changeDirection(Position3D inputPosition, const float deltaT)
{

}

void Boss::shoot(Position3D inputPosition, const float deltaT)
{
    Position3D shootingVector;
    shootingVector.origin = position.origin;
    shootingVector.orientation = (position.origin - inputPosition.origin);
    bullets->insert(new Bullet(shootingVector, 3.0f, 3.0f, false));
}



