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



    //void moveProjectile(unsigned int projectileId, float distance,
                       // float windowWidth, float windowHeight);

public:

    Enemy(Position3D position);

    void shoot(Position3D inputPosition, float deltaT);

};
#endif //CGPROJECT2023_24_ENEMY_H
