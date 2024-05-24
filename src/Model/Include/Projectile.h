//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_PROJECTILE_H
#define CGPRJ2023_24_PROJECTILE_H

#include "glm/glm.hpp"


class Projectile {

private:

    glm::vec4 position;
    glm::vec4 direction;
    int size;
    int speed;

public:

    Projectile(glm::vec4 position, glm::vec4 direction, int size, int speed);

    glm::vec4 getPosition();
    glm::vec4 getDirection();
    int getSize();
    int getSpeed();
};


#endif //CGPRJ2023_24_PROJECTILE_H
