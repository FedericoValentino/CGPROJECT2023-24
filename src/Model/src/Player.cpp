//
// Created by feder34 on 21/05/24.
//


#include "../Include/Player.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Player::Player() {
    this->direction = glm::vec4 {0,0,0,1};
    this->hp = 10;
    this->projectiles.clear();
}

void Player::shoot() {
    projectiles.push_back(new Projectile(glm::vec4{0,0,0,1}, direction, glm::vec4{1,1,1,1}, 1));
}

void Player::changeDirection(float roll, float pitch, float yaw) {
    glm::mat4 Rx = glm::rotate(glm::mat4(1), glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1), glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1), glm::radians(roll), glm::vec3(0, 0, 1));
    glm::mat4 R = Rx * Ry * Rz;
    direction = R * direction;
}

