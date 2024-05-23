//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_PROJECTILE_H
#define CGPRJ2023_24_PROJECTILE_H

#include "glm/glm.hpp"


class Projectile {

public:

    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 size;
    int speed;

    Projectile(glm::vec4 position, glm::vec4 direction, glm::vec4 size, int speed);

};


#endif //CGPRJ2023_24_PROJECTILE_H
