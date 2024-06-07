//
// Created by Pudduh on 21/05/24.
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

    void changePosition(Position3D inputPosition, float deltaT);

    void changeDirection(Position3D inputPosition, float deltaT);

    void shoot(Position3D inputPosition, float deltaT);

};


#endif //CGPRJ2023_24_PLAYER_H
