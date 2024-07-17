//
// Created by Puddu && Niccolo on 21/05/24.
//

#ifndef CGPROJECT2023_24_ENEMY_H
#define CGPROJECT2023_24_ENEMY_H
#include <glm/glm.hpp>
#include "glm/gtx/intersect.hpp"
#include <unordered_map>
#include "Bullet.h"
#include "Player.h"
#include "Position3D.h"
#include "Plane.h"



class Enemy : public Plane{

protected:

public:

    Enemy(const Position3D& position);

    std::shared_ptr<Bullet> shoot(const Position3D& inputPosition, float deltaT) override;

    void evasive(float deltaT);

    void climb(int direction, float climbRate, float deltaT);

    void moveTowardsPoint(const Position3D& point, float deltaT) override;

    void changeDirection(const Position3D& inputPosition, float deltaT) override;

};
#endif //CGPROJECT2023_24_ENEMY_H
