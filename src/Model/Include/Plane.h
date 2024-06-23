//
// Created by Pudduh on 07/06/2024.
//

#ifndef CGPRJ2023_24_PLANE_H
#define CGPRJ2023_24_PLANE_H

#include <set>
#include "Position3D.h"
#include "Bullet.h"
#include "PlaneType.h"

/**
 * Superclass containing all the common parameters and methods needed by a plane object
 */
class Plane {

protected:
    float rotationSpeed;
    float translationSpeed;
public:
    float getTranslationSpeed() const;

    float getRotationSpeed() const;

protected:
    std::set<Bullet*>* bullets;
    PLANETYPE type;
    int hp;
    bool dead;

public:

    Position3D position;

    Plane();

    void planeHit(Bullet bullet);

    void clearBullet(Bullet* own);

    virtual void shoot(Position3D inputPosition, float deltaT) = 0;

    void moveTowardsPoint(Position3D point, float deltaT);

    void changePosition(Position3D inputPosition, float deltaT);

    void changeDirection(Position3D inputPosition, float deltaT);

    bool checkDistance3D(glm::vec3 center, glm::vec3 point, PLANETYPE plane);

    Position3D getPosition() const;
    float getSpeed();
    std::set<Bullet*> getBullets();
    int getHP();
    bool getDead();
    PLANETYPE getType();
};


#endif //CGPRJ2023_24_PLANE_H
