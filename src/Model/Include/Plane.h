//
// Created by Pudduh on 07/06/2024.
//

#ifndef CGPRJ2023_24_PLANE_H
#define CGPRJ2023_24_PLANE_H

#include <set>
#include "Position3D.h"
#include "Bullet.h"

enum PLANETYPE{
    PLAYER,
    ENEMY,
    BOSS
};


class Plane {

protected:
    Position3D position;
    float speed;
    std::set<Bullet*>* bullets;
    PLANETYPE type;
    int hp;
    bool dead;

public:

    Plane();

    void planeHit(Bullet bullet);

    void clearBullet(Bullet* own);

    virtual void shoot(Position3D position) = 0;

    virtual void changePosition(Position3D position) = 0;

    virtual void changeDirection(Position3D position) = 0;

    Position3D getPosition();
    float getSpeed();
    std::set<Bullet*> getBullets();
    int getHP();
    bool getDead();
    PLANETYPE getType();
};


#endif //CGPRJ2023_24_PLANE_H
