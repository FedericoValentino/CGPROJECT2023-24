//
// Created by Pudduh on 21/05/24.
//


#include <cstdio>
#include <algorithm>
#include "../Include/Player.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Player::Player() {
    position.origin = glm::vec4(0.0f, 8.40f, 0.0f, 1.0f);
    position.orientation = glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
    speed = 2;
    hp = 5;
    type = PLAYER;
    dead = false;
}

void Player::changePosition(Position3D inputPosition, const float deltaT) {
    float x = inputPosition.origin.x * speed * deltaT;
    float y = inputPosition.origin.y * speed * deltaT;
    float z = inputPosition.origin.z * speed * deltaT;
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
    position.origin= position.origin * T;

}

void Player::changeDirection(Position3D inputPosition, const float deltaT) {
    glm::mat4 Rx = glm::rotate(glm::mat4(1), glm::radians(inputPosition.orientation.x * deltaT), glm::vec3(1, 0, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1), glm::radians(inputPosition.orientation.y * deltaT), glm::vec3(0, 1, 0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1), glm::radians(inputPosition.orientation.z * deltaT), glm::vec3(0, 0, 1));
    glm::mat4 R = Rx * Ry * Rz;
    position.orientation = position.orientation * R;
}

void Player::shoot(Position3D inputPosition, const float deltaT) {
    auto* toShoot = new Bullet(position, 1.0f, 2.0f, false);
    bullets->insert(toShoot);
}





