//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_PLAYER_H
#define CGPRJ2023_24_PLAYER_H

#include "glm/glm.hpp"
#include "vector"
#include "Bullet.h"
#include "Plane.h"


class Player : public Plane{

public:

    Player();

    void changePosition(Position3D position, const float deltaT);

    void changeDirection(Position3D position, const float deltaT);

    void shoot(Position3D position, const float deltaT);

};


#endif //CGPRJ2023_24_PLAYER_H
