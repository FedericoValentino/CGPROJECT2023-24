//
// Created by Pudduh && Niccolo on 21/05/24.
//

#ifndef CGPRJ2023_24_BOSS_H
#define CGPRJ2023_24_BOSS_H
#include "Plane.h"
#include "Enemy.h"

class Boss: public Plane{

public:
    Boss(const Position3D& position);

    void bossMovement(const Position3D& playerPosition, float deltaT);

    void circularMovement(const Position3D& center, float deltaT);

    std::shared_ptr<Bullet> shoot(const Position3D& inputPosition, float deltaT) override;

    void changeDirection(const Position3D& playerPosition, float deltaT) override;

    virtual void changePosition(const Position3D& inputPosition, float deltaT) override;

    void verticalMovement(float deltaT);

};


#endif //CGPRJ2023_24_BOSS_H
