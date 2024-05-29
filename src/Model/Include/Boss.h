//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_BOSS_H
#define CGPRJ2023_24_BOSS_H
#include "Enemy.h"

class Boss: public Enemy{
public:
    Boss(const Position3D& position3D,const float speed, bool debug=false);
public:
    virtual void move(const Player& player,const float deltaT) override;
};


#endif //CGPRJ2023_24_BOSS_H
