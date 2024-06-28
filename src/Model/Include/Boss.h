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

    void bossMovement(Position3D playerPosition, float deltaT);

    void circularMovement(Position3D center, float deltaT);

    Bullet* shoot(Position3D inputPosition, float deltaT) override;


};


#endif //CGPRJ2023_24_BOSS_H
