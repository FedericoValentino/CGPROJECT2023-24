//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_BOSS_H
#define CGPRJ2023_24_BOSS_H
#include "Plane.h"
#include "Enemy.h"

class Boss: public Plane{

public:
    Boss(Position3D position);

    void changePosition(Position3D position);

    void changeDirection(Position3D position);

    void shoot(Position3D position);


};


#endif //CGPRJ2023_24_BOSS_H
