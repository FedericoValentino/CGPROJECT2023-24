//
// Created by feder34 on 21/05/24.
//

#include "../Include/Boss.h"


Boss::Boss(Position3D position){
    this->position = position;
    speed = 1;
    type = BOSS;
    hp = 10;
    dead = false;
}

void Boss::changePosition(Position3D position) {
}

void Boss::changeDirection(Position3D position) {
}

void Boss::shoot(Position3D position) {

}



