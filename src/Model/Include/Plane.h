//
// Created by Pudduh on 07/06/2024.
//

#ifndef CGPRJ2023_24_PLANE_H
#define CGPRJ2023_24_PLANE_H

#include <set>
#include <memory>
#include "Position3D.h"
#include "Bullet.h"
#include "PlaneType.h"

/**
 * Superclass containing all the common parameters and methods needed by a plane object
 */
class Plane {

public:
    float getTranslationSpeed() const;

    float getRotationSpeed() const;

protected:
    float rotationSpeed;
    float translationSpeed;
    std::shared_ptr<std::set<std::shared_ptr<Bullet>>> bullets;
    PLANETYPE_UTILITY type;
    int hp;
    bool dead;
    bool avoidBuilding;
    float elapsedTime;
    float evasionTimer;

public:

    Position3D position;

    Plane();

    void planeHit(const Bullet& bullet);

    void clearBullet(std::shared_ptr<Bullet> own);

    virtual std::shared_ptr<Bullet> shoot(const Position3D& inputPosition, float deltaT) = 0;

    void moveTowardsPoint(const Position3D& point, float deltaT);

    void changePosition(const Position3D& inputPosition, float deltaT);

    virtual void changeDirection(const Position3D& inputPosition, float deltaT);

    bool checkDistance3D(const glm::vec3& center,const glm::vec3& point, PLANETYPE_UTILITY desiredRadius);

    void roll(int direction, float deltaT);

    void climb(int direction, float climbRate, float deltaT);

    void evasive(float deltaT);

    Position3D getPosition() const;
    float getSpeed();
    std::set<std::shared_ptr<Bullet>> getBullets();
    int getHP() const;
    bool getDead() const;
    PLANETYPE_UTILITY getType();
    void timePasses(const float deltaT);

    void setAvoidBuilding(bool avoiding);
};


#endif //CGPRJ2023_24_PLANE_H
