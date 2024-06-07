//
// Created by feder34 on 21/05/24.
//


#include <cstdio>
#include <algorithm>
#include "../Include/Player.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



/**
 * Allows to modify the Player's orientation, through a matrix product
 * @param roll - rotation around z
 * @param pitch - rotation around x
 * @param yaw - rotation around y
 */
/*void Player::changeDirection(float roll, float pitch, float yaw) {
    glm::mat4 Rx = glm::rotate(glm::mat4(1), glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1), glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1), glm::radians(roll), glm::vec3(0, 0, 1));
    glm::mat4 R = Rx * Ry * Rz;
    direction = R * direction;
}*/




