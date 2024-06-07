//
// Created by feder34 on 21/05/24.
//

#ifndef CGPROJECT2023_24_ENEMY_H
#define CGPROJECT2023_24_ENEMY_H
#include <glm/glm.hpp>
#include <unordered_map>
#include "Bullet.h"
#include "Player.h"
#include "Position3D.h"
#include "Plane.h"
#include <mutex>
#include <thread>


class Enemy : public Plane{



    //void moveProjectile(unsigned int projectileId, float distance,
                       // float windowWidth, float windowHeight);

public:

    Enemy(Position3D position);

    void changePosition(Position3D position, const float deltaT);

    void changeDirection(Position3D position, const float deltaT);

    void shoot(Position3D position, const float deltaT);

};
#endif //CGPROJECT2023_24_ENEMY_H
