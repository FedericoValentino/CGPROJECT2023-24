//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_PLAYER_H
#define CGPRJ2023_24_PLAYER_H

#include "glm/glm.hpp"
#include "vector"
#include "Projectile.h"


class Player {

public:

    glm::vec4 direction;
    int hp;
    std::vector<Projectile*> projectiles;

    void shoot();

    void changeDirection(float roll,float pitch, float yaw);


    Player();
};


#endif //CGPRJ2023_24_PLAYER_H
