//
// Created by feder34 on 21/05/24.
//

#include "../Include/Projectile.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Projectile::Projectile(glm::vec4 position, glm::vec4 direction, int size, int speed) {
    this->position = position;
    this->direction = direction;
    this->size = size;
    this->speed = speed;
}

glm::vec4 Projectile::getPosition() {
    return position;
}

glm::vec4 Projectile::getDirection() {
    return direction;
}

int Projectile::getSize() {
    return size;
}

int Projectile::getSpeed() {
    return speed;
}
