//
// Created by feder34 on 21/05/24.
//

#include "../Include/Projectile.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Projectile::Projectile(glm::vec4 position, glm::vec4 direction, glm::vec4 size, int speed) {
    this->position = position;
    this->direction = direction;
    this->size = size;
    this->speed = speed;
}
