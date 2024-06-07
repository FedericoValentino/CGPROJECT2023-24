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

private:
    Position3D position;
    float speed;
    std::set<Bullet*> bullets;
    int hp;
    bool hit;

public:

    //Plane(float speed);

    void shoot();

    void planeHit(Bullet bullet);

    void clearBullet(Bullet* own);

    void changePosition();

    void changeDirection();

    Position3D getPosition();
    const float getSpeed();
    std::set<Bullet*> getBullets();
    int getHP();
    bool getHit();
};


#endif //CGPRJ2023_24_PLANE_H
