//
// Created by feder34 on 21/05/24.
//


#include <cstdio>
#include <algorithm>
#include "../Include/Player.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Player::Player() {
    this->direction = glm::vec4 {0,0,0,1};
    this->hp = 10;
    this->projectiles.clear();
}

/**
 * Adds a new projectile in the Player's projectiles vector, with same Player's position and direction
 */
void Player::shoot() {
    //projectiles.push_back(new Projectile(glm::vec4{0,0,0,1}, direction, 1, 1));
}

/**
 * Allows to modify the Player's orientation, through a matrix product
 * @param roll - rotation around z
 * @param pitch - rotation around x
 * @param yaw - rotation around y
 */
void Player::changeDirection(float roll, float pitch, float yaw) {
    glm::mat4 Rx = glm::rotate(glm::mat4(1), glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1), glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1), glm::radians(roll), glm::vec3(0, 0, 1));
    glm::mat4 R = Rx * Ry * Rz;
    direction = R * direction;
}


/**
 * Checks whether the projectile has hit the player's position, which is the center of the screen, and
 * applies to the Player's health pool based on its own size
 * @param projectile - the projectile to test
 */
void Player::hit(Projectile* projectile) {
    if (projectile->getPosition3D().origin == glm::vec3{0,0,0})
        hp -= projectile->getSize();
}

/**
 * Checks if a player is dead
 * @return true if Player's hp are depleted
 */
bool Player::dead() {
    return hp <= 0;
}

/**
 * Removes a projectile from Player's projectiles when it hits an enemy or exits the screen.
 * @param own - the projectile in question
 */
void Player::clearProjectile(Projectile own) {
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [&own](Projectile * i) {return i && (i == &own);}), projectiles.end());
}

glm::vec4 Player::getDirection() {
    return direction;
}

int Player::getHp() {
    return hp;
}

std::vector<Projectile *> Player::getProjectiles() {
    return projectiles;
}

