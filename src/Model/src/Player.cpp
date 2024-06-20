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
    position.rotation = glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
    speed = 2;
    hp = 5;
    type = PLAYER;
    dead = false;
}

/**
 * Modifies the Player's position according to the new position
 * @param newPosition the new origin position
 */
void Player::setPosition(glm::vec3 newPosition) {
    position.origin = newPosition;
}

/**
 * Modifies the Player's orientation according to the new direction
 * @param newDirection the new direction
 */
void Player::setOrientation(glm::vec3 newOrientation) {
    position.rotation = newOrientation;
}

/**
 * Adds the projectile to be shot in the Player's bullet set
 * @param inputPosition the Player's position
 * @param deltaT time
 */
void Player::shoot(Position3D inputPosition, const float deltaT) {
    auto* toShoot = new Bullet(inputPosition, PLAYER, false);
    bullets->insert(toShoot);
}





