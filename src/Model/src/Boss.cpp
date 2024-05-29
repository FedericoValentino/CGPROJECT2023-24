//
// Created by feder34 on 21/05/24.
//

#include "../Include/Boss.h"

Boss::Boss(const Position3D& position3D,const float speed, bool debug=false):
    Enemy(position3D,speed,debug);

void Boss::move(const Player& player,const float deltaT)
{
    //...
}
