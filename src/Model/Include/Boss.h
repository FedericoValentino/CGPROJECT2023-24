//
// Created by Pudduh && Niccolo on 21/05/24.
//

#ifndef CGPRJ2023_24_BOSS_H
#define CGPRJ2023_24_BOSS_H
#include "Plane.h"
#include "Enemy.h"

class Boss: public Plane{

public:
    Boss(Position3D position);

    void changePosition(Position3D inputPosition, float deltaT);

    void changeDirection(Position3D inputPosition, float deltaT);

    void shoot(Position3D inputPosition, float deltaT);


};


#endif //CGPRJ2023_24_BOSS_H
